#include <iostream>
#include <fstream>
using namespace std;


//data
struct WorkStation {
	int type;//
	float x_pos;
	float y_pos;
	int status;
	int material;
	int production;

public:
	WorkStation(int type, float x_pos, float y_pos, int status, int material, int prduction) {
		this->type = type;
		this->x_pos = x_pos;
		this->y_pos = y_pos;
		this->status = status;
		this->material = material;
		this->production = production;

	}
};

struct Robot {
	int stationID;
	int carryType;
	float timeValCoef;
	float crashValCoef;
	float angularSpeed;
	float linearSpeed_X;
	float linearSpeed_Y;
	float direction;
	float position_X;
	float position_Y;

public:
	Robot(int stationID,
		int carryType,
		float timeValCoef,
		float crashValCoef,
		float angularSpeed,
		float linearSpeed_X,
		float linearSpeed_Y,
		float direction,
		float position_X,
		float position_Y)
	{
		this->stationID = stationID;
		this->carryType = carryType;
		this->timeValCoef = timeValCoef;
		this->crashValCoef = crashValCoef;
		this->angularSpeed = angularSpeed;
		this->linearSpeed_X = linearSpeed_X;
		this->linearSpeed_Y = linearSpeed_Y;
		this->direction = direction;
		this->position_X = position_X;
		this->position_Y = position_Y;
	}


};

