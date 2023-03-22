#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <math.h>
#include "DataStruct.cpp"
#define FramePerSecond 50
using namespace std;

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
	materialPrice[1].buyPrice = 3000, Price[1].sellPrice = 6000;
	materialPrice[2].buyPrice = 4400, Price[2].sellPrice = 7600;
	materialPrice[3].buyPrice = 5800, Price[3].sellPrice = 9200;
	materialPrice[4].buyPrice = 15400, Price[4].sellPrice = 22500;
	materialPrice[5].buyPrice = 17200, Price[5].sellPrice = 25000;
	materialPrice[6].buyPrice = 19200, Price[6].sellPrice = 27500;
	materialPrice[7].buyPrice = 76000, Price[7].sellPrice = 105000;
}

double DistanceCalcu(float Ax, float Ay, float Bx, float By) {
	return sqrt(pow(Ax-Bx,2)+ pow(Ay - By, 2))
}

//Only Speed Up£¬No Down 
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

double ProfitCalcu_A2B_StraightLine(WorkStation A, WorkStation B, int material) {
	int oriBuyPrice = materialPrice[material].buyPrice;
	int oriSellPrice = materialPrice[material].sellPrice;
	double distance = DistanceCalcu(A.x_pos, A.y_pos, B.x_pos, B.y_pos);
	int frameNum = path_FrameNumCalcu(distance, true);
	return SellPriceCalcu_no_Crash(oriSellPrice, frameNum) - oriBuyPrice;
}