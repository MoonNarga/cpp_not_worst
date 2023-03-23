#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
#include "DataStruct.cpp"
#define FramePerSecond 50
#define Frame123 50
#define Frame456 500
#define Frame7 1000
using namespace std;


//会变成乱码吗

//all never test
extern MaterialPrice materialPrice[8];//begin from index 1

extern double emptyWeight = 0.45 * 0.45 * M_PI * 20;
extern double loadingWeight = 0.53 * 0.53 * M_PI * 20;
extern double maxSpeed = 6;
extern double minSpeed = -2;
extern double maxRotate = M_PI;
extern double minRotate = -M_PI;
extern double maxForce = 250;
extern double maxMomentForce = 50;




bool initPrice() {
	materialPrice[1].buyPrice = 3000, materialPrice[1].sellPrice = 6000;
	materialPrice[2].buyPrice = 4400, materialPrice[2].sellPrice = 7600;
	materialPrice[3].buyPrice = 5800, materialPrice[3].sellPrice = 9200;
	materialPrice[4].buyPrice = 15400, materialPrice[4].sellPrice = 22500;
	materialPrice[5].buyPrice = 17200, materialPrice[5].sellPrice = 25000;
	materialPrice[6].buyPrice = 19200, materialPrice[6].sellPrice = 27500;
	materialPrice[7].buyPrice = 76000, materialPrice[7].sellPrice = 105000;
}

double DistanceCalcu(WorkStation A, WorkStation B) {
	return sqrt(pow(A.x_pos - B.x_pos, 2) + pow(A.y_pos - B.y_pos, 2));
}

// double DistanceCalcu(float Ax, float Ay, float Bx, float By) {
// 	return sqrt(pow(Ax-Bx,2)+ pow(Ay - By, 2))
// }

//Only Speed Up��No Down 
int Path_FrameNumCalcu(double distance,bool loading) {
	double weight;
	if (loading == true) {
		weight = loadingWeight;			
	}
	if (loading == false) {
		weight = emptyWeight;
	}
	double acc = maxForce / weight;							//F=ma
	double speedUpTime = maxSpeed / acc;							//v=at
	double speedUpDistance = acc * pow(speedUpTime, 2) / 2;			//S=at^2/2
	if (distance < speedUpDistance) return sqrt(2 * distance / acc) * FramePerSecond;	//S=at^2/2
	else return ((distance - speedUpDistance) / maxSpeed + speedUpTime) * FramePerSecond;
}

double SellPriceCalcu_no_Crash(int oriSellPrice, int frameNum) {
	int x = frameNum;
	int maxX = 9000;
	float minRate = 0.8;
	double timeValCoef = (1 - sqrt(1 - pow(1 - (x / maxX), 2))) * (1 - minRate) + minRate;
	if (x < maxX)return  oriSellPrice * timeValCoef;
	return oriSellPrice * minRate;
}

double ProfitCalcu_A2B_StraightLine(double distance,int frameNum, int material) {
	int oriBuyPrice = materialPrice[material].buyPrice;
	int oriSellPrice = materialPrice[material].sellPrice;
	//double distance = DistanceCalcu(A.x_pos, A.y_pos, B.x_pos, B.y_pos);
	//int frameNum = path_FrameNumCalcu(distance, true);
	return SellPriceCalcu_no_Crash(oriSellPrice, frameNum) - oriBuyPrice;
}


bool DirectionDistance_sort(DirectionDistance* A,DirectionDistance* B) {
	return A->profitRate > B->profitRate;

}

int GetBestFrame(int workStationID) {
	WorkStation station = workStation[workStationID];
	if (station.type == 4)
		return station.material_vector[1][0].directFrame + station.material_vector[2][0].directFrame;
	else if (station.type == 5)
		return station.material_vector[1][0].directFrame + station.material_vector[3][0].directFrame;
	else if (station.type == 6)
		return station.material_vector[2][0].directFrame + station.material_vector[3][0].directFrame;
}

int GetBestProfit(int workStationID) {
	WorkStation station = workStation[workStationID];
	if (station.type == 4)
		return station.material_vector[1][0].profit + station.material_vector[2][0].profit;
	else if (station.type == 5)
		return station.material_vector[1][0].profit + station.material_vector[3][0].profit;
	else if (station.type == 6)
		return station.material_vector[2][0].profit + station.material_vector[3][0].profit;
}

void A2X_Material_sort(int workStationID, int directType) {
	DirectionDistance directStation;
	int MaterialType=directType;
	if (directType > 7)MaterialType = workStation[workStationID].type;
	int Sell_type;
	for (Sell_type = 0;Sell_type < workStation_type[directType].size();Sell_type++) {//�ù���̨��ÿһ������̨�ļ���
		directStation.directID = workStation_type[directType][Sell_type];
		directStation.distance = DistanceCalcu(workStation[workStationID], workStation[directStation.directID]);
		directStation.directFrame = Path_FrameNumCalcu(directStation.distance, true);
		if (directType >= 4 && directType <= 6)directStation.totalFrame = GetBestFrame(directStation.directID) + directStation.directFrame;
		else directStation.totalFrame = directStation.directFrame;

		
		directStation.profit = ProfitCalcu_A2B_StraightLine(directStation.distance, directStation.directFrame, MaterialType);
		if (directType >= 4 && directType <= 6)directStation.totalProfit = GetBestProfit(directStation.directID) + directStation.profit;
		else directStation.totalProfit = directStation.profit;
		
		directStation.profitRate = directStation.totalProfit / directStation.totalFrame;
		workStation[workStationID].material_vector[directType].push_back(directStation);


	}
	//����̨����
	sort(workStation[workStationID].material_vector[directType].begin(), workStation[workStationID].material_vector[directType].end(), DirectionDistance_sort);
}

void MaterialBestCalcu(int StationID) {
	int Material_type = workStation[StationID].type;
	int Mx = 0;
	int My = 0;
	if (Material_type == 4) {
		Mx = 1;My = 2;
	}
	if (Material_type == 5) {
		Mx = 1;My = 3;
	}
	if (Material_type == 6) {
		Mx = 2;My = 3;
	}

	A2X_Material_sort(StationID, Mx);
	A2X_Material_sort(StationID, My);
	workStation[StationID].materialFrame = workStation[StationID].material_vector[Mx][0].totalFrame + workStation[StationID].material_vector[My][0].totalFrame + 2 * Frame123;
	workStation[StationID].materialProfit= workStation[StationID].material_vector[Mx][0].totalProfit + workStation[StationID].material_vector[My][0].totalProfit;

}

void ProfitCalcu() {
	int Material_type = 0;
	int W_ID = 0;
	int StationID = 0;
	int Sell_type = 0;
	
	for (Material_type = 1;Material_type < 8;Material_type++) {//����ÿһ����
		for (W_ID = 0;W_ID < workStation_type[Material_type].size();W_ID++) {//����ÿһ������̨
			StationID = workStation_type[Material_type][W_ID];
			//���㵽����̨�����Ž�
			A2X_Material_sort(StationID, 9);
			if (Material_type == 7)A2X_Material_sort(StationID, 8);

			//��·�����Ž�
			if (Material_type <= 3) {
				workStation[StationID].pathProfit = workStation[StationID].material_vector[Material_type][0].profit;
				workStation[StationID].pathProfitRate = workStation[StationID].pathProfit / Frame123;
			}
			else if (Material_type <= 6) {
				MaterialBestCalcu(StationID);
				workStation[StationID].pathProfit = workStation[StationID].materialProfit + workStation[StationID].material_vector[Material_type][0].profit;
				workStation[StationID].productFrame = workStation[StationID].materialFrame + workStation[StationID].material_vector[Material_type][0].directFrame + Frame456;
				workStation[StationID].pathProfitRate = workStation[StationID].pathProfit / workStation[StationID].productFrame;
			}
		

		}
		
	}
}


void PriorityCalcu() {

}










