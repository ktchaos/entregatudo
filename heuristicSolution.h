#ifndef HEURISTICSOLUTION_H
#define HEURISTICSOLUTION_H
#include <iostream>
#include <vector>
#include "fleet.h"
#include "client.h"
#include "travel.h"
#include "greedysolution.h"

using namespace std;

class HeuristicSolution {
    private:
        Fleet fleet;
        vector<Client> clients;

        vector<int> demandOfClients;
        vector<int> outsourcingCost;
        vector< vector<int> > matrixOfCosts;

        int solutionCost;

        public:
            GreedySolution solution;
            HeuristicSolution(Fleet fleet, vector<Client> clients, vector<int> demandOfClients, vector<int> outsourcingCost, vector< vector<int> > matrixOfCosts);
            
            void applySingleRouteNeighbor(vector<Travel> travels, Fleet fleet, vector<Client> clients, vector< vector<int> > matrixOfCosts);
            void applyMultipleRoutesNeighbor(vector<Travel> travels, Fleet fleet, vector<Client> clients, vector< vector<int> > matrixOfCosts);
            void applyOutsourcingNeighbor(vector<Travel> travels, Fleet fleet, vector<Client> clients, vector<int> outsourcingCost, vector< vector<int> > matrixOfCosts);
            void applyVND(vector<Travel> travels, Fleet fleet, vector<Client> clients, vector<int> outsourcingCost, vector< vector<int> > matrixOfCosts);
            
            void apply2OptNeighbor(vector<Travel>& travels, Fleet& fleet, vector<Client>& clients, vector< vector<int> > matrixOfCosts);
            void applyReinsertionNeighbor(vector<Travel>& travels, Fleet& fleet, vector<Client>& clients, vector< vector<int> > matrixOfCosts);
            void updateTotalCost();
            int calculateTravelCost(Travel& travel, vector< vector<int> > matrixOfCosts);
};

#endif