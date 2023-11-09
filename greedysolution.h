#ifndef GREEDYSOLUTION_H
#define GREEDYSOLUTION_H
#include "travel.h"
#include "client.h"

using namespace std;

struct GreedySolution {
    vector<Travel> travels;
    vector<Client> outsourcedClients;
    int totalCostOfTravels;
    int totalCostsFromUsageOfFleet;
    int totalCostOfOutsourcing;
    int total;

    GreedySolution() {}
    GreedySolution(vector<Travel> _travels, vector<Client> _outsourcedClients, int _totalCostOfTravels, int _totalCostsFromUsageOfFleet, int _totalCostOfOutsourcing, int _total) : travels(_travels), outsourcedClients(_outsourcedClients), totalCostOfTravels(_totalCostOfTravels), totalCostsFromUsageOfFleet(_totalCostsFromUsageOfFleet), totalCostOfOutsourcing(_totalCostOfOutsourcing), total(_total) {}
};

#endif