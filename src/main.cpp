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


fstream fout("status.log", ios::out);
stringstream ss;
vector<WorkStation>workStation;
vector<Robot>robot;

//vector<WorkStation> priority_workStation;
queue<robotOrder> orderQueue[4];

materialPrice Price[8];//begin from index 1

int frameID;
int money;
int workStationNum;
double emptyWeight = 0.45 * 0.45 * M_PI * 20;
double loadingWeight = 0.53 * 0.53 * M_PI * 20;


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

bool init() {
    Price[1].buyPrice = 3000, Price[1].sellPrice = 6000;
    Price[2].buyPrice = 4400, Price[2].sellPrice = 7600;
    Price[3].buyPrice = 5800, Price[3].sellPrice = 9200;
    Price[4].buyPrice = 15400, Price[4].sellPrice = 22500;
    Price[5].buyPrice = 17200, Price[5].sellPrice = 25000;
    Price[6].buyPrice = 19200, Price[6].sellPrice = 27500;
    Price[7].buyPrice = 76000, Price[7].sellPrice = 105000;

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
