#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <math.h>
#include "DataStruct.cpp"
using namespace std;


//fstream fout("status.log", ios::out);
//stringstream ss;
//extern vector<WorkStation>workStation;//index [0,49]
//extern vector<Robot>robot;
//
////vector<WorkStation> priority_workStation;
//
//extern vector<int> workStation_type[10];//index [1,9],只存ID
//queue<RobotOrder> orderQueue[4];
//
//
//int frameID;
//int money;
//extern int workStationNum;



bool readUntilOK() {
    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        fout << line;
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        //do something
    }
    return false;
}


bool readMap() {//never test
    char line[1024];
    int x, y;
    char c;
    for (y = 100;y >= 0;y--) {
        for (x = 0;x < 101;x++) {
            cin >> c;
            if (c >= '1' || c <= '9') {
                workStation.push_back(WorkStation(c-'0', x * 0.5 - 0.25, y * 0.5 - 0.25, -1, 0, 0));
                workStation_type[c - '0'].push_back(workStation.size() - 1);//最新一位的ID存入type
            }
            if (c == 'A') {
                robot.push_back(Robot(-1, 0, 0, 0, 0, 0, 0, 0, x * 0.5 - 0.25, y * 0.5 - 0.25));
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
    puts("OK");
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
