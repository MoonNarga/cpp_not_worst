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

char line[1024];
int frameID;
int money;
int workStationNum;
vector<Robot> robots;
vector<WorkStation> workStation; // index [0,49]

deque<pair<int, int>> taskQueue; // first: from, second: to
vector<queue<int>> sellQueue(8);

bool readUntilOK() {
    while (fgets(line, sizeof line, stdin)) {
        fout << line;
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        // do something
    }
    return false;
}

bool updateStatus() {
    // update money
    scanf("%d", &money);
    // update workStation
    for (int i = 0; i < workStationNum; i++) {
        int type, time, material, production;
        double x, y;
        scanf("%d%lf%lf%d%d%d", &type, &x, &y, &time, &material, &production);
        workStation[i].update(material, production);
        if (production == 1) {
            if (!sellQueue[type].empty()) {
                taskQueue.push_back(make_pair(i, sellQueue[type].front()));
                sellQueue[type].pop();
            }
        }
    }
    // update robots
    for (int robotId = 0; robotId < 4; robotId++) {
        int workStation, material;
        double time, clash, vTheta, vX, vY, orient, x, y;
        scanf("%d%d%lf%lf%lf%lf%lf%lf%lf%lf", &workStation, &material, &time,
              &clash, &vTheta, &vX, &vY, &orient, &x, &y);
        robots[robotId].update(workStation, material, time, clash, vTheta, vX,
                               vY, orient, x, y);
    }
    fgets(line, sizeof(line), stdin);
    if (line[0] == 'O' && line[1] == 'K') {
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
        updateStatus();
        if (taskQueue.size() <= 20) {
            vector<pair<int, int>> temp = priorityCalculation.getTask();
            for (auto i : temp) {
                sellQueue[i.first].push(i.second);
            }
        }

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
