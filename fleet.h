#ifndef FLEET_H
#define FLEET_H

struct Fleet {
    int capacity; // Q
    int cost; // r
    int numberOfVehicles; // k
    int minOfTravels; // L

    Fleet() {}
    Fleet(int _capacity, int _cost, int _numberOfVehicles, int _minOfTravels) : capacity(_capacity), cost(_cost), numberOfVehicles(_numberOfVehicles), minOfTravels(_minOfTravels) {}
};

#endif