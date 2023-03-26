#define _USE_MATH_DEFINES
#include "PriorityCalculation.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// char line[1024];
int frameID;
int money;
int workStationNum;
vector<Robot> robots;
vector<WorkStation> workStation; // index [0,49]

deque<pair<int, int>> taskQueue; // first: from, second: to
vector<queue<int>> sellQueue(8);

int dispatch() {
    for (int i = 0; i < 4; i++) {
        if (robots[i].numTask() < 2) {
            robots[i].getTask(taskQueue);
        }
    }
    return 0;
}

bool updateStatus() {
    // update money
    scanf("%d", &money);
    scanf("%d", &workStationNum);
    // update workStation
    for (int i = 0; i < workStationNum; i++) {
        int type, time, material, production;
        double x, y;
        scanf("%d%lf%lf%d%d%d", &type, &x, &y, &time, &material, &production);
        workStation[i].update(material, production);
        if (production == 1 && workStation[i].preStatus != production) {
            if (!sellQueue[type].empty()) {
                taskQueue.push_front(make_pair(i, sellQueue[type].front()));
                sellQueue[type].pop();
            }
        }
    }

    dispatch();

    // update robots
    for (int robotId = 0; robotId < 4; robotId++) {
        int workStation, material;
        double time, clash, vTheta, vX, vY, orient, x, y;
        scanf("%d%d%lf%lf%lf%lf%lf%lf%lf%lf", &workStation, &material, &time,
              &clash, &vTheta, &vX, &vY, &orient, &x, &y);
        robots[robotId].update(workStation, material, time, clash, vTheta, vX,
                               vY, orient, x, y);
    }
    string temp;
    cin >> temp;
    if (temp == "OK") {
        fout << temp << endl;
        return true;
    }
    return false;
}

int main() {
    PriorityCalculation priorityCalculation(workStation, robots);
    puts("OK");
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF) {
        fout << frameID << endl;
        printf("%d\n", frameID);

        if (taskQueue.size() <= 20) {
            priorityCalculation.getTask(sellQueue);
            for (int i = 1; i < 8; ++i) {
                if (!sellQueue[i].empty()) {
                    fout << i << " " << sellQueue[i].front() << endl;
                }
            }
        }

        updateStatus();

        // int lineSpeed = 3;
        // double angleSpeed = 1.5;
        // for (int robotId = 0; robotId < 4; robotId++) {
        //     printf("forward %d %d\n", robotId, lineSpeed);
        //     printf("rotate %d %f\n", robotId, angleSpeed);
        // }
        printf("OK\n");
        fflush(stdout);
    }
    return 0;
}
