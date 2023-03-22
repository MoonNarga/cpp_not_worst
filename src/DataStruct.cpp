#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <math.h>
using namespace std;

//data
struct WorkStation {
	int type;//
	float x_pos;
	float y_pos;
	int status;
	int material;
	int production;
	
	//新增变量
	//对于1-3 material全0
	//对于4-6 分别对原料数组进行排序
	vector<DirectionDistance> material[8];//index [1,6]存原料台 index[self]存目标台排序 用sort by totalProfit/totalFrame
	DirectionDistance sellStation;//存售卖台，该台只会是最近的台子
	int materialFrame;//存 取原料总帧数（最优）
	double materialProfit;//存 生产原料总利润（最优）
	double pathProfit;//=用 原料总帧数+售卖总帧数 和 原料总利润+售卖利润 算
public:
	WorkStation(int type, float x_pos, float y_pos, int status, int material, int prduction) {
		this->type = type;
		this->x_pos = x_pos;
		this->y_pos = y_pos;
		this->status = status;
		this->material = material;
		this->production = production;

	}
};

struct Robot {
	int stationID;
	int carryType;
	float timeValCoef;
	float crashValCoef;
	float angularSpeed;
	float linearSpeed_X;
	float linearSpeed_Y;
	float direction;
	float position_X;
	float position_Y;

public:
	Robot(int stationID,
		int carryType,
		float timeValCoef,
		float crashValCoef,
		float angularSpeed,
		float linearSpeed_X,
		float linearSpeed_Y,
		float direction,
		float position_X,
		float position_Y)
	{
		this->stationID = stationID;
		this->carryType = carryType;
		this->timeValCoef = timeValCoef;
		this->crashValCoef = crashValCoef;
		this->angularSpeed = angularSpeed;
		this->linearSpeed_X = linearSpeed_X;
		this->linearSpeed_Y = linearSpeed_Y;
		this->direction = direction;
		this->position_X = position_X;
		this->position_Y = position_Y;
	}


};


#define forward 0
#define rotate 1
#define buy 2
#define sell 3
#define destroy 4
struct RobotOrder {
	int orderNum;
	int robotID;
	float arg;
public:
	RobotOrder(int orderNum, int robotID, float arg) {
		this->orderNum = orderNum;
		this->robotID = robotID;
		this->arg = arg;
	}
};

struct MaterialPrice {
	int buyPrice;
	int sellPrice;
public:
	MaterialPrice(int buyPrice, int sellPrice) {
		this->buyPrice = buyPrice;
		this->sellPrice = sellPrice;
	}
};



struct DirectionDistance {
	int directID;	//存目标台ID
	double distance;//存目标距离
	int directFrame;//存到达帧数
	int totalFrame;//到达帧数+取原料最优总帧数
	double profit;  //存该短线路利润
	double totalProfit;//存该短线路利润+取原料最优总利润
	double profitRate;//利润率 perFrame


};


fstream fout("status.log", ios::out);
stringstream ss;
extern vector<WorkStation>workStation;//index [0,49]
extern vector<Robot>robot;

//vector<WorkStation> priority_workStation;
extern vector<int> workStation_type[10];//index [1,9],只存ID
queue<RobotOrder> orderQueue[4];

int frameID;
int money;
extern int workStationNum;
