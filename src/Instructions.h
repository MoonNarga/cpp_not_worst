#include <cmath>
#include <deque>
#include <iostream>
#include <pthread.h>
#include <string>
#define FORWARD 1
#define ROTATE 2
#define BUY 3
#define SELL 4
#define DESTROY 5



using namespace std;

class Behaviour {
  private:
    int behaviour, wordStationID;

  public:
    Behaviour(int beha, int id) {
        this->behaviour = beha;
        this->wordStationID = id;
    };
    ~Behaviour(){};
};

queue <Behaviour> PickBeha;
queue <Behaviour> SellBeha;