#ifndef DATA_H
#define DATA_H
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Data {
    private:
        int numberOfClients; // n
        int numberOfVehicles; // k
        int capacityOfVehicle; // Q
        int minOfDeliveriesToBeDone; // L
        int costOfVehicle; // r

        vector<int> demandOfClients; // d
        vector<int> outsourcingCost; // p
        vector< vector<int> > matrixOfCosts; // c

        public:
            
            Data(string path);

            // print methods
            void printNumberOfClients();
            void printNumberOfVehicles();
            void printCapacityOfVehicle();
            void printMinOfDeliveriesToBeDone();
            void printCostOfVehicle();
            void printDemandOfClients();
            void printOutsourcingCost();
            void printMatrixOfCosts();
            void print(std::vector<int> const &input);
            void printMatrix(const vector< vector<int> >& matrix);

            // Get methods
            int getNumberOfClients();
            int getNumberOfVehicles();
            int getCapacityOfVehicle();
            int getMinOfDeliveriesToBeDone();
            int getCostOfVehicle();

            vector<int> getDemandOfClients();
            vector<int> getOutsourcingCost();
            vector< vector<int> > getMatrixOfCosts();
};

#endif