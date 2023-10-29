#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "data.h"
#include "heuristicSolution.h"

using namespace std;

int main() {
    // Retrieve data from txt file
    // Data data = Data("instancias/n9k5_A.txt");
    // Data data = Data("instancias/n9k5_B.txt");
    // Data data = Data("instancias/n9k5_C.txt");
    // Data data = Data("instancias/n9k5_D.txt");
    // Data data = Data("instancias/n14k5_A.txt");
    // Data data = Data("instancias/n14k5_B.txt");
    // Data data = Data("instancias/n14k5_C.txt");
    // Data data = Data("instancias/n14k5_D.txt");
    Data data = Data("instancias/n22k3_A.txt");
    // Data data = Data("instancias/n22k3_B.txt");
    // Data data = Data("instancias/n22k3_C.txt");
    // Data data = Data("instancias/n22k3_D.txt");
    // Data data = Data("instancias/n31k5_A.txt");
    // Data data = Data("instancias/n31k5_B.txt");
    // Data data = Data("instancias/n31k5_C.txt");
    // Data data = Data("instancias/n31k5_D.txt");
    // Data data = Data("instancias/n43k6_A.txt");
    // Data data = Data("instancias/n43k6_B.txt");
    // Data data = Data("instancias/n43k6_C.txt");
    // Data data = Data("instancias/n43k6_D.txt");
    // Data data = Data("instancias/n64k9_A.txt");
    // Data data = Data("instancias/n64k9_B.txt");
    // Data data = Data("instancias/n64k9_C.txt");
    // Data data = Data("instancias/n64k9_D.txt");
    // Data data = Data("instancias/n120k7_A.txt");
    // Data data = Data("instancias/n120k7_B.txt");
    // Data data = Data("instancias/n120k7_C.txt");
    // Data data = Data("instancias/n120k7_D.txt");
    // Data data = Data("instancias/n199k17_A.txt");
    // Data data = Data("instancias/n199k17_B.txt");
    // Data data = Data("instancias/n199k17_C.txt");
    // Data data = Data("instancias/n199k17_D.txt");

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

    // heuristicSolution.applySingleRouteNeighbor(heuristicSolution.solution.travels, fleet, clientsInSolution, data.getMatrixOfCosts());
    // cout << "teste = " << heuristicSolution.solution.total << endl;

    // heuristicSolution.applyMultipleRoutesNeighbor(heuristicSolution.solution.travels, fleet, clientsInSolution, data.getMatrixOfCosts());
    // cout << "teste 2 = " << heuristicSolution.solution.total << endl;

    // heuristicSolution.applyOutsourcingNeighbor(heuristicSolution.solution.travels, fleet, clientsInSolution, formattedOutsourcingCost, data.getMatrixOfCosts());
    // cout << "teste 2 = " << heuristicSolution.solution.total << endl;

    heuristicSolution.applyVND(heuristicSolution.solution.travels, fleet, clientsInSolution, formattedOutsourcingCost, data.getMatrixOfCosts());
    cout << "teste 2 = " << heuristicSolution.solution.total << endl;

    return 0;
}