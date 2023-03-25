#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct DirectionDistance {
    int directID;       //存目标台ID
    double distance;    //存目标距离
    int directFrame;    //存到达帧数
    int totalFrame;     //到达帧数+取原料最优总帧数
    double profit;      //存该短线路利润
    double totalProfit; //存该短线路利润+取原料最优总利润
    double profitRate;  //利润率 perFrame
};

// data
struct WorkStation {
    int type; //
    int ID;
    double x_pos;
    double y_pos;
    int status;
    int material;
    int production;
    int gettingMutex[7];
    int SellMutex;

    //新增变量
    //对于1-3 material全0
    //对于4-6 分别对原料数组进行排序
    vector<DirectionDistance>
        material_vector[10]; // index [1,6]存原料台 index[self]存目标台排序
                             // 用sort by totalProfit/totalFrame
    DirectionDistance sellStation; //存售卖台，该台只会是最近的台子
    int materialFrame;             //存 取原料总帧数（最优）
    int productFrame;              //存 生产原料总帧数
    double materialProfit;         //存 生产原料总利润（最优）
    double pathProfit; //=用 原料总帧数+售卖总帧数+生产原料总帧数 和
                       //原料总利润+售卖利润 算
    double pathProfitRate; //总线路利润率

  public:
    WorkStation(int type, int ID,double x_pos, double y_pos, int status, int material,
                int prduction) {
        this->type = type;
        this->ID=ID;
        this->x_pos = x_pos;
        this->y_pos = y_pos;
        this->status = status;
        this->material = material;
        this->production = production;
        this->SellMutex=1;
        for(int i=1;i<7;i++){
            this->gettingMutex[i]=1;
        }
    }

    int update(int material, int production) {
        this->material = material;
        this->production = production;
        return 0;
    }

    double getCoordX() { return this->x_pos; }
    double getCoordY() { return this->y_pos; }
};

struct MaterialPrice {
    int buyPrice;
    int sellPrice;

  public:
    MaterialPrice() {}
    MaterialPrice(int buyPrice, int sellPrice) {
        this->buyPrice = buyPrice;
        this->sellPrice = sellPrice;
    }
};
