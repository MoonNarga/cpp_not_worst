#pragma once
#include <cmath>
#include <deque>
#include <iostream>
#include <pthread.h>
#include <string>

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

  public:
    Robot(int id, int x, int y, vector<WorkStation> &WS) : workStations(WS) {
        this->id = id;
        coordX = x;
        coordY = y;
    }

    void calculateVTheta() {
        if (targetID == -1) {
            vTheta = 0;
            return;
        }
        double x = workStations[targetID].getCoordX() - coordX;
        double y = workStations[targetID].getCoordY() - coordY;
        double theta = atan2(y, x);
        vTheta = theta - orient;
        if (vTheta > M_PI) {
            vTheta -= 2 * M_PI;
        } else if (vTheta < -M_PI) {
            vTheta += 2 * M_PI;
        }
    }

    void calculateV() {
        if (targetID == -1) {
            vX = 0;
            vY = 0;
            return;
        }
        double x = workStations[targetID].getCoordX() - coordX;
        double y = workStations[targetID].getCoordY() - coordY;
        double theta = atan2(y, x);
        double v = sqrt(x * x + y * y);
        vX = v * cos(theta);
        vY = v * sin(theta);
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
        return 0;
    }

    int setTarget(int targetID) {
        this->targetID = targetID;
        return 0;
    }
};
