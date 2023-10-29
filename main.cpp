#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include "data.h"
#include "heuristicSolution.h"

using namespace std;

float calculateGAP(int value, int optimumValue) {
    double result = ((static_cast<double>(value) - optimumValue) / optimumValue);
    return (result * 100);
}

int main() {
    // Retrieve data from txt file
    Data data = Data("instancias/n9k5_A.txt", 428);
    // Data data = Data("instancias/n9k5_B.txt", 506);
    // Data data = Data("instancias/n9k5_C.txt", 559);
    // Data data = Data("instancias/n9k5_D.txt", 408);
    // Data data = Data("instancias/n14k5_A.txt", 471);
    // Data data = Data("instancias/n14k5_B.txt", 565);
    // Data data = Data("instancias/n14k5_C.txt", 569);
    // Data data = Data("instancias/n14k5_D.txt", 471);
    // Data data = Data("instancias/n22k3_A.txt", 605);
    // Data data = Data("instancias/n22k3_B.txt", 777);
    // Data data = Data("instancias/n22k3_C.txt", 777);
    // Data data = Data("instancias/n22k3_D.txt", 605);
    // Data data = Data("instancias/n31k5_A.txt", 650);
    // Data data = Data("instancias/n31k5_B.txt", 933);
    // Data data = Data("instancias/n31k5_C.txt", 939);
    // Data data = Data("instancias/n31k5_D.txt", 656);
    // Data data = Data("instancias/n43k6_A.txt", 801);
    // Data data = Data("instancias/n43k6_B.txt", 1203);
    // Data data = Data("instancias/n43k6_C.txt", 1208);
    // Data data = Data("instancias/n43k6_D.txt", 802);
    // Data data = Data("instancias/n64k9_A.txt", 934);
    // Data data = Data("instancias/n64k9_B.txt", 1503);
    // Data data = Data("instancias/n64k9_C.txt", 1510);
    // Data data = Data("instancias/n64k9_D.txt", 932);
    // Data data = Data("instancias/n120k7_A.txt", 1029);
    // Data data = Data("instancias/n120k7_B.txt", 2052);
    // Data data = Data("instancias/n120k7_C.txt", 2040);
    // Data data = Data("instancias/n120k7_D.txt", 1046);
    // Data data = Data("instancias/n199k17_A.txt", 1672);
    // Data data = Data("instancias/n199k17_B.txt", 3302);
    // Data data = Data("instancias/n199k17_C.txt", 3301);
    // Data data = Data("instancias/n199k17_D.txt", 1672);

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

    auto startTimeHeuristicSolution = chrono::high_resolution_clock::now();
    HeuristicSolution heuristicSolution = HeuristicSolution(
        fleet,
        clientsInSolution,
        demandOfClients,
        formattedOutsourcingCost,
        data.getMatrixOfCosts()
    );

    cout << "Heuristic Solution = "<< heuristicSolution.solution.total << endl;
    auto endTimeHeuristicSolution = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsedTime = endTimeHeuristicSolution - startTimeHeuristicSolution;
    double elapsedSeconds = elapsedTime.count();
    cout << "Tempo de execução Guloso: " << elapsedSeconds << " segundos" << endl;
    cout << "GAP - HS: " << calculateGAP(heuristicSolution.solution.total, data.getOptimumValue()) << " segundos" << endl;

    // heuristicSolution.applySingleRouteNeighbor(heuristicSolution.solution.travels, fleet, clientsInSolution, data.getMatrixOfCosts());
    // cout << "teste = " << heuristicSolution.solution.total << endl;

    // heuristicSolution.applyMultipleRoutesNeighbor(heuristicSolution.solution.travels, fleet, clientsInSolution, data.getMatrixOfCosts());
    // cout << "teste 2 = " << heuristicSolution.solution.total << endl;

    // heuristicSolution.applyOutsourcingNeighbor(heuristicSolution.solution.travels, fleet, clientsInSolution, formattedOutsourcingCost, data.getMatrixOfCosts());
    // cout << "teste 2 = " << heuristicSolution.solution.total << endl;

    auto startTimeVND = chrono::high_resolution_clock::now();
    heuristicSolution.applyVND(heuristicSolution.solution.travels, fleet, clientsInSolution, formattedOutsourcingCost, data.getMatrixOfCosts());
    cout << "VND = " << heuristicSolution.solution.total << endl;

    auto endTimeVND = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsedTimeVND = endTimeVND - startTimeVND;
    double elapsedSecondsVND = elapsedTimeVND.count();
    cout << "Tempo de execução VND: " << elapsedSecondsVND << " segundos" << endl;

    // float result = static_cast<float>((heuristicSolution.solution.total - 428) / 428);
    // float gap = (result * 100);

    cout << "GAP - VND: " << calculateGAP(heuristicSolution.solution.total, data.getOptimumValue()) << " segundos" << endl;

    return 0;
}