#include <cmath>
#include <deque>
#include <iostream>
#include <pthread.h>
#include <string>

using namespace std;

class Behaviour {
  private:
    int behaviour, wordStationID, priority;

  public:
    Behaviour(int beha, int id, int prior) {
        this->behaviour = beha;
        this->wordStationID = id;
        this->priority = prior;
    };
    ~Behaviour(){};
};
