#include "DataStruct.cpp"
#include "action.cpp"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

fstream fout("status.log", ios::out);
vector<WorkStation> workStation;
vector<Robot> robot;

bool readUntilOK() {
    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        fout << line;
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        // do something
    }
    return false;
}

bool readMap() { //δ����
    char line[1024];
    int x, y;
    char c;
    for (y = 100; y >= 0; y--) {
        for (x = 0; x < 101; x++) {
            cin >> c;
            if (c >= '1' || c <= '9') {
                workStation.push_back(WorkStation((c - '0'), x * 0.5 - 0.25,
                                                  y * 0.5 - 0.25, -1, 0, 0));
            }
            if (c == 'A') {
                robot.push_back(Robot(-1, 0, 0, 0, 0, 0, 0, 0, x * 0.5 - 0.25,
                                      y * 0.5 - 0.25));
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
