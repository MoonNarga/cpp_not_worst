#pragma once
#include <cmath>
#include <deque>
#include <iostream>
#include <pthread.h>
#include <string>
#define BUY 1
#define SELL 2
#define MOVE 3

using namespace std;

class Robot {
  private:
    int id;
    int workStation = -1, materialCarry = 0;
    double time = 0, clash = 0;
    double vX = 0, vY = 0, vForward = 0, vTheta = 0, orient = 0;
    double coordX, coordY;
    int targetID = -1;
    vector<WorkStation> &workStations; // index [0,49]
    deque<pair<int, int>> currentTask; // first: to, second: op

  public:
    Robot(int id, int x, int y, vector<WorkStation> &WS) : workStations(WS) {
        this->id = id;
        coordX = x;
        coordY = y;
    }

    void calculateV() {
        if (targetID == -1) {
            vTheta = 0;
            return;
        }
        double x = workStations[targetID].getCoordX() - coordX;
        double y = workStations[targetID].getCoordY() - coordY;
        double theta = atan2(y, x);
        double thetaDiff = theta - orient;

        if (thetaDiff > 0) {
            vTheta = -M_PI;
        } else if (thetaDiff < 0) {
            vTheta = M_PI;
        }
        if (thetaDiff > M_PI_2 || thetaDiff < -M_PI_2) {
            vForward = 0;
        } else {
            vForward = 6;
        }
    }

    int update(int workStation, int material, double time, double clash,
               double vTheta, double vX, double vY, double orient, double x,
               double y) {
        this->workStation = workStation;
        this->materialCarry = material;
        this->time = time;
        this->clash = clash;
        this->vTheta = vTheta;
        this->vX = vX;
        this->vY = vY;
        this->orient = orient;
        this->coordX = x;
        this->coordY = y;
        if (workStation == currentTask.front().first) {
            if (currentTask.front().second == BUY) {
                printf("buy %d\n", id);
            } else if (currentTask.front().second == SELL) {
                printf("sell %d\n", id);
            }
            currentTask.pop_front();
        }
        calculateV();
        printf("rotate %d %lf\n", id, vTheta);
        printf("forward %d %lf\n", id, vForward);
        return 0;
    }

    int setTarget(int targetID) {
        this->targetID = targetID;
        return 0;
    }

    int numTask() { return currentTask.size(); }

    int getTask(deque<pair<int, int>> &taskQueue) {
        currentTask.push_back(make_pair(taskQueue.front().first, BUY));
        currentTask.push_back(make_pair(taskQueue.front().second, SELL));
        taskQueue.pop_front();
        return 0;
    }
};
