#include "Instructions.h"
#include <cmath>
#include <deque>
#include <iostream>
#include <pthread.h>
#include <string>

using namespace std;

class Robot {
  private:
    int workStation = -1, materialCarry = 0;
    double time = 0, clash = 0;
    double vX = 0, vY = 0, vTheta = 0, orient = 0;
    double coordX, coordY;

  public:
    Robot(int x, int y) {
        coordX = x;
        coordY = y;
    }

    
};