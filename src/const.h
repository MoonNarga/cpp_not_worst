#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;

double emptyWeight = 0.45 * 0.45 * M_PI * 20;
double loadingWeight = 0.53 * 0.53 * M_PI * 20;
double maxSpeed = 6;
double minSpeed = -2;
double maxRotate = M_PI;
double minRotate = -M_PI;
double maxForce = 250;
double maxMomentForce = 50;
