#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "data.h"

using namespace std;

int main()
{
    Data data = Data("instancias/n9k5_A.txt");
    // Data data = Data("instancias/n9k5_A.txt");
    // Data data = Data("instancias/n9k5_D.txt");
    // Data data = Data("instancias/n14k5_A.txt");
    // Data data = Data("instancias/n14k5_D.txt");
    // Data data = Data("instancias/n22k3_A.txt");
    // Data data = Data("instancias/n22k3_D.txt");

    data.printNumberOfClients();
    data.printNumberOfVehicles();
    data.printCapacityOfVehicle();
    data.printMinOfDeliveriesToBeDone();
    data.printCostOfVehicle();
    data.printDemandOfClients();
    data.printOutsourcingCost();
    data.printMatrixOfCosts();

    return 0;
}