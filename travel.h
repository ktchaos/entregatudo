#ifndef TRAVEL_H
#define TRAVEL_H

#include <vector>
#include "client.h"

using namespace std;

struct Travel {
    vector<Client> clientsDone;
    int totalCost;

    Travel() {}
    Travel(vector<Client> _clientsDone, int _totalCost) : clientsDone(_clientsDone), totalCost(_totalCost) {}
};

#endif