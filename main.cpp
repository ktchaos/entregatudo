#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "data.h"
#include "heuristicSolution.h"

using namespace std;

int main() {
    // Retrieve data from txt file
    Data data = Data("instancias/n9k5_A.txt");
    // Data data = Data("instancias/n9k5_A.txt");
    // Data data = Data("instancias/n9k5_D.txt");
    // Data data = Data("instancias/n14k5_A.txt");
    // Data data = Data("instancias/n14k5_D.txt");
    // Data data = Data("instancias/n22k3_A.txt");
    // Data data = Data("instancias/n22k3_D.txt");

    // data.printNumberOfClients();
    // data.printNumberOfVehicles();
    // data.printCapacityOfVehicle();
    // data.printMinOfDeliveriesToBeDone();
    // data.printCostOfVehicle();
    // data.printDemandOfClients();
    // data.printOutsourcingCost();
    // data.printMatrixOfCosts();

    int numberOfClients = data.getNumberOfClients();
    vector<int> demandOfClients = data.getDemandOfClients();

    vector<Client> clientsInSolution;

    // Recupera a lista de clientes
    for(int i = 0; i < numberOfClients; i++) {
        int demandOfCurrentClient = demandOfClients[i];
        Client currentClient(i + 1, demandOfCurrentClient, false);
        clientsInSolution.push_back(currentClient);
    }

    Fleet fleet(
        data.getCapacityOfVehicle(),
        data.getCostOfVehicle(),
        data.getNumberOfVehicles(),
        data.getMinOfDeliveriesToBeDone()
    );
    vector<int> placeholder(1, 0);
    vector<int> outsourcingData = data.getOutsourcingCost();
    vector<int> formattedOutsourcingCost;

    formattedOutsourcingCost.reserve(placeholder.size() + outsourcingData.size());
    formattedOutsourcingCost.insert(formattedOutsourcingCost.end(), placeholder.begin(), placeholder.end());
    formattedOutsourcingCost.insert(formattedOutsourcingCost.end(), outsourcingData.begin(), outsourcingData.end());

    HeuristicSolution heuristicSolution = HeuristicSolution(
        fleet,
        clientsInSolution,
        demandOfClients,
        formattedOutsourcingCost,
        data.getMatrixOfCosts()
    );

    cout << heuristicSolution.solution.total << endl;

    heuristicSolution.applySingleRouteNeighbor(heuristicSolution.solution.travels, fleet, clientsInSolution, data.getMatrixOfCosts());
    cout << "mov-1otp = " << heuristicSolution.solution.total << endl;

    heuristicSolution.applyMultiRouteNeighbor(heuristicSolution.solution.travels, fleet, clientsInSolution, data.getMatrixOfCosts());
    cout << "mov-2otp = " << heuristicSolution.solution.total << endl;

    return 0;
}