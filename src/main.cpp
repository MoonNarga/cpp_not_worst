#define _USE_MATH_DEFINES
#include "DataStruct.h"
#include "Robot.h"
#include "const.h"
// #include "PriorityCalculation.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void GetMaterial(int ID);
int GetNearestID(int workStationID, int material);

int frameID;
int money;
int workStationNum;
vector<WorkStation> workStation; // index [0,49]
vector<Robot> robots;
vector<int> workStation_type[10]; // index [1,9],只存ID
vector<int> PriorityStation;

#define FramePerSecond 50
#define Frame123 50
#define Frame456 500
#define Frame7 1000

// 会变成乱码吗

// all never test
MaterialPrice materialPrice[8]; // begin from index 1

void PutsPriorityLog() {
    fout << "Priority Calculation Log:" << endl;
    int i = 0;
    for (i = 0; i < workStationNum; i++) {
        fout << workStation[i].type << " " << workStation[i].ID << " "
             << workStation[i].x_pos << " " << workStation[i].y_pos << endl;
        fout << "status:" << workStation[i].status << endl;
        fout << "material" << workStation[i].material << endl;
        fout << "production:" << workStation[i].production << endl;
        fout << "gettingMutex:" << workStation[i].gettingMutex << endl;
        fout << "SellMutex:" << workStation[i].SellMutex << endl;
        fout << "pathProfitRate:" << workStation[i].pathProfitRate << endl
             << endl;
    }
    fout << "-----------------Log End------------------------" << endl;
}

void initPrice() {
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

// Only Speed Up��No Down
int Path_FrameNumCalcu(double distance, bool loading) {
    double weight;
    if (loading == true) {
        weight = loadingWeight;
    }
    if (loading == false) {
        weight = emptyWeight;
    }
    double acc = maxForce / weight;                         // F=ma
    double speedUpTime = maxSpeed / acc;                    // v=at
    double speedUpDistance = acc * pow(speedUpTime, 2) / 2; // S=at^2/2
    if (distance < speedUpDistance)
        return sqrt(2 * distance / acc) * FramePerSecond; // S=at^2/2
    else
        return ((distance - speedUpDistance) / maxSpeed + speedUpTime) *
               FramePerSecond;
}

double SellPriceCalcu_no_Crash(int oriSellPrice, int frameNum) {
    int x = frameNum;
    int maxX = 9000;
    float minRate = 0.8;
    double timeValCoef =
        (1 - sqrt(1 - pow(1 - (x / maxX), 2))) * (1 - minRate) + minRate;
    if (x < maxX)
        return oriSellPrice * timeValCoef;
    return oriSellPrice * minRate;
}

double ProfitCalcu_A2B_StraightLine(double distance, int frameNum,
                                    int material) {
    int oriBuyPrice = materialPrice[material].buyPrice;
    int oriSellPrice = materialPrice[material].sellPrice;
    // double distance = DistanceCalcu(A.x_pos, A.y_pos, B.x_pos, B.y_pos);
    // int frameNum = path_FrameNumCalcu(distance, true);
    return SellPriceCalcu_no_Crash(oriSellPrice, frameNum) - oriBuyPrice;
}

bool DirectionDistance_sort(DirectionDistance A, DirectionDistance B) {
    return A.profitRate > B.profitRate;
}

int GetBestFrame(int workStationID) {
    WorkStation station = workStation[workStationID];
    if (station.type == 4) {
        if (GetNearestID(workStationID, 1) == -1 ||
            GetNearestID(workStationID, 2) == -1)
            return 0;
        return station.material_vector[1][0].directFrame +
               station.material_vector[2][0].directFrame;
    }

    else if (station.type == 5) {
        if (GetNearestID(workStationID, 1) == -1 ||
            GetNearestID(workStationID, 3) == -1)
            return 0;
        return station.material_vector[1][0].directFrame +
               station.material_vector[3][0].directFrame;
    } else if (station.type == 6) {
        if (GetNearestID(workStationID, 2) == -1 ||
            GetNearestID(workStationID, 3) == -1)
            return 0;
        return station.material_vector[2][0].directFrame +
               station.material_vector[3][0].directFrame;
    }
    return 0;
}

int GetBestProfit(int workStationID) {
    WorkStation station = workStation[workStationID];
    if (station.type == 4) {
        if (GetNearestID(workStationID, 1) == -1 ||
            GetNearestID(workStationID, 2) == -1)
            return 0;
        return station.material_vector[1][0].profit +
               station.material_vector[2][0].profit;
    }

    else if (station.type == 5) {
        if (GetNearestID(workStationID, 1) == -1 ||
            GetNearestID(workStationID, 3) == -1)
            return 0;
        return station.material_vector[1][0].profit +
               station.material_vector[3][0].profit;
    } else if (station.type == 6) {
        if (GetNearestID(workStationID, 2) == -1 ||
            GetNearestID(workStationID, 3) == -1)
            return 0;
        return station.material_vector[2][0].profit +
               station.material_vector[3][0].profit;
    }
    return 0;
}

void A2X_Material_sort(int workStationID,
                       int directType) { // 计算A到某个台的最优解
    DirectionDistance directStation;
    int MaterialType = directType;
    if (directType > 7)
        MaterialType = workStation[workStationID].type;
    int Sell_type;
    for (Sell_type = 0; Sell_type < workStation_type[directType].size();
         Sell_type++) { // 该工作台对每一个台的计算
        directStation.directID = workStation_type[directType][Sell_type];
        directStation.distance = DistanceCalcu(
            workStation[workStationID], workStation[directStation.directID]);
        
        directStation.directFrame =
            Path_FrameNumCalcu(directStation.distance, true);
        if (directType >= 4 && directType <= 6) //
            directStation.totalFrame = GetBestFrame(directStation.directID) +
                                       directStation.directFrame;
        else
            directStation.totalFrame = directStation.directFrame;

        directStation.profit = ProfitCalcu_A2B_StraightLine(
            directStation.distance, directStation.directFrame, MaterialType);
        if (directType >= 4 && directType <= 6)
            directStation.totalProfit =
                GetBestProfit(directStation.directID) + directStation.profit;
        else
            directStation.totalProfit = directStation.profit;

        directStation.profitRate =
            directStation.totalProfit / directStation.totalFrame;
        workStation[workStationID].material_vector[MaterialType].push_back(
            directStation);
    }
    // 台排序
    if (workStation[workStationID].material_vector[directType].size() != 0) {
        sort(workStation[workStationID].material_vector[directType].begin(),
             workStation[workStationID].material_vector[directType].end(),
             DirectionDistance_sort);
    }
}

void CannotGet(int StationID) {
    workStation[StationID].materialFrame = 0;
    workStation[StationID].materialProfit = 0;
    workStation[StationID].pathProfitRate = -1;
    return;
}

void MaterialBestCalcu(int StationID) {

    int Material_type = workStation[StationID].type;
    if (Material_type <= 3) {
        workStation[StationID].materialFrame = 0;
        workStation[StationID].materialProfit = 0;
        return;
    }
    int Mx = 0;
    int My = 0;
    int Mz = 0;
    if (Material_type == 4) {
        Mx = 1;
        My = 2;
    }
    if (Material_type == 5) {
        Mx = 1;
        My = 3;
    }
    if (Material_type == 6) {
        Mx = 2;
        My = 3;
    }
    if (Material_type == 7) {
        Mx = 4;
        My = 5;
        Mz = 6;
    }

    A2X_Material_sort(StationID, Mx);
    A2X_Material_sort(StationID, My);

    if (GetNearestID(StationID, Mx) == -1 ||
        GetNearestID(StationID, My) == -1) { // xy不可达
        CannotGet(StationID);
        return;
    }
    int directIDx = workStation[StationID].material_vector[Mx][0].directID;
    int directIDy = workStation[StationID].material_vector[My][0].directID;
    if (workStation[directIDx].pathProfitRate == -1 ||
        workStation[directIDy].pathProfitRate == -1) { // xy无法生产  456
        CannotGet(StationID);
        return;
    }
    workStation[StationID].materialFrame =
        workStation[StationID].material_vector[Mx][0].totalFrame +
        workStation[StationID].material_vector[My][0].totalFrame;
    workStation[StationID].materialProfit =
        workStation[StationID].material_vector[Mx][0].totalProfit +
        workStation[StationID].material_vector[My][0].totalProfit;

    if (Mz != 0) {

        A2X_Material_sort(StationID, Mz);
        if (GetNearestID(StationID, Mz) == -1) { // z不可达
            CannotGet(StationID);
            return;
        }
        int directIDz = workStation[StationID].material_vector[Mz][0].directID;
        if (workStation[directIDz].pathProfitRate == -1) { // z无法生成
            CannotGet(StationID);
            return;
        }
        workStation[StationID].materialFrame +=
            workStation[StationID].material_vector[Mz][0].totalFrame +
            3 * Frame456;
        workStation[StationID].materialProfit +=
            workStation[StationID].material_vector[Mz][0].totalProfit;
    }
    if (Mz == 0) {
        workStation[StationID].materialFrame += 2 * Frame123;
    }
}

int GetNearestID(int workStationID, int material) {
    if (workStation[workStationID].material_vector[material].size() == 0) {
        return -1; // 不存在
    } else
        return workStation[workStationID].material_vector[material][0].directID;
}

void PathProfitRateCalcu(int StationID, int Material_type) {

    if (GetNearestID(StationID, Material_type) == -1||workStation[StationID].pathProfitRate == -1) {
        workStation[StationID].pathProfit = 0;
        workStation[StationID].productFrame = 0;
        workStation[StationID].pathProfitRate = -1;
        return;
    }

    workStation[StationID].pathProfit =
        workStation[StationID].materialProfit +
        workStation[StationID].material_vector[Material_type][0].profit;

    workStation[StationID].productFrame =
        workStation[StationID].materialFrame +
        workStation[StationID].material_vector[Material_type][0].directFrame;

    if (Material_type <= 3) {
        workStation[StationID].productFrame += Frame123;
    }
    if (Material_type == 7) {

        workStation[StationID].productFrame += Frame7;
    } else
        workStation[StationID].productFrame += Frame456;

    workStation[StationID].pathProfitRate =
        workStation[StationID].pathProfit / workStation[StationID].productFrame;
}

void ProfitCalcu() { // 计算利润，最终得到pathProfitRate
    int Material_type = 0;
    int W_ID = 0;
    int StationID = 0;
    int Sell_type = 0;

    for (Material_type = 1; Material_type < 8;
         Material_type++) { // 对于每一类型
        for (W_ID = 0; W_ID < workStation_type[Material_type].size();
             W_ID++) { // 对于每一个工作台
            StationID = workStation_type[Material_type][W_ID];
            // 先算到售卖台的最优解
            A2X_Material_sort(StationID, 9);
            if (Material_type == 7)
                A2X_Material_sort(StationID, 8);

            // 算路径最优解
            MaterialBestCalcu(StationID); //原料最优解
            if (GetNearestID(StationID, Material_type) == -1) { //售卖链
                workStation[StationID].pathProfitRate=-2;
            } else {
                PathProfitRateCalcu(StationID, Material_type);
            }
        }
    }
}

bool PrioritySort(WorkStation A, WorkStation B) {
    return A.pathProfitRate > B.pathProfitRate;
}

void PriorityCalcu() {                            // 计算优先级
    initPrice();
    ProfitCalcu();                                // 计算利润
    vector<WorkStation> stationSort(workStation); // 返回优先级数组

    sort(stationSort.begin(), stationSort.end(), PrioritySort);
    for (auto i : stationSort) {
        if(i.pathProfitRate>0&&i.type<=7)
            PriorityStation.push_back(i.ID);
    }
}

void SellOrder(int ID) { // ？

    SellBeha.push(Behaviour(SELL, ID));
}

void PickOrder(int fromID, int toID) {
    PickBeha.push(Behaviour(BUY, fromID));
    PickBeha.push(Behaviour(SELL, toID));
}

void GetOrProduct(int toID, int fromID, int material_type) {
    if (workStation[fromID].production) {
        PickOrder(fromID, toID);
        workStation[toID].gettingMutex[material_type]--;
    } else
        GetMaterial(fromID);
}

void GetMaterial(int ID) {
    int Material_type = workStation[ID].type;
    int Mx = 0;
    int My = 0;
    int Mz = 0;
    int mat[7];
    int i = 0;
    for (i = 1; i < 7; i++)
        mat[i] = (workStation[ID].material >> (i - 1)) %
                 2; // 原料格状态 index【1,6】

    if (Material_type == 4) {
        Mx = 1;
        My = 2;
    }
    if (Material_type == 5) {
        Mx = 1;
        My = 3;
    }
    if (Material_type == 6) {
        Mx = 2;
        My = 3;
    }
    if (Material_type == 7) {
        Mx = 4;
        My = 5;
        Mz = 6;
    }
    if (mat[Mx] == 0 && workStation[ID].gettingMutex[Mx])
        GetOrProduct(ID, workStation[ID].material_vector[Mx][0].directID, Mx);
    if (mat[My] == 0 && workStation[ID].gettingMutex[My])
        GetOrProduct(ID, workStation[ID].material_vector[My][0].directID, My);
    if (Mz != 0) {
        if (mat[Mz] == 0 && workStation[ID].gettingMutex[Mz])
            GetOrProduct(ID, workStation[ID].material_vector[Mz][0].directID,
                         Mz);
    }
}

void GetWorkOrder() {
    int i = 0;
    int ID = 0;
    int maxID = 0;

    for (i = 0; i < PriorityStation.size() && PickBeha.size() < 20;
         i++) { // 队列长度限定20
        ID = PriorityStation[i];
        if (workStation[ID].production && workStation[ID].SellMutex) {
            SellOrder(ID);
            workStation[ID].SellMutex--;
        }
        GetMaterial(ID);
    }
}

// mutex ：机器人操作
//  sellMutex:机器人取到后++
//  gettingMutex:机器人送到后++

bool readUntilOK() {
    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        fout << line;
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        // do something
    }
    return false;
}

bool readMap() { // never test
    char line[1024];
    int x, y;
    char c;
    int id = 0;
    for (y = 100; y >= 0; y--) {
        for (x = 0; x < 101; x++) {
            cin >> c;
            if (c >= '1' && c <= '9') {
                workStation.push_back(WorkStation(c - '0', id++, x * 0.5 - 0.25,
                                                  y * 0.5 - 0.25, -1, 0, 0));
                workStation_type[c - '0'].push_back(id -
                                                    1); // 最新一位的ID存入type
            }
            if (c == 'A') {
                robots.push_back(Robot(x * 0.5 - 0.25, y * 0.5 - 0.25));
            }
            if (c == 'O') {
                cin >> c;
                if (c == 'K') {
                    fout << "OK" << endl;
                    return true;
                }
            }
            fout << c;
        }
    }
    return false;
}

int main() {
    readMap();

    PriorityCalcu();

    puts("OK");

    PutsPriorityLog();
    // while更新状态，调用GetWorkOrder()
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF) {
        readUntilOK();
        printf("%d\n", frameID);
        int lineSpeed = 3;
        double angleSpeed = 1.5;
        for (int robotId = 0; robotId < 4; robotId++) {
            printf("forward %d %d\n", robotId, lineSpeed);
            printf("rotate %d %f\n", robotId, angleSpeed);
        }
        // printf("OK\n", frameID);
        printf("OK\n");
        fflush(stdout);
    }
    return 0;
}
