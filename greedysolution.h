#ifndef GREEDYSOLUTION_H
#define GREEDYSOLUTION_H
#include "travel.h"

using namespace std;

struct GreedySolution {
    vector<Travel> travels;
    int totalCostOfTravels;
    int totalCostsFromUsageOfFleet;
    int totalCostOfOutsourcing;
    int total;

    GreedySolution() {}
    GreedySolution(vector<Travel> _travels, int _totalCostOfTravels, int _totalCostsFromUsageOfFleet, int _totalCostOfOutsourcing, int _total) : travels(_travels), totalCostOfTravels(_totalCostOfTravels), totalCostsFromUsageOfFleet(_totalCostsFromUsageOfFleet), totalCostOfOutsourcing(_totalCostOfOutsourcing), total(_total) {}
};

#endif