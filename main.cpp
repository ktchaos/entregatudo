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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "FILE NOT FOUND" << std::endl;
        return 1;
    }

    const string& instanceFile = argv[1];

    // Creating file for compute results
    string filename = "result.txt";
    ofstream resultsArchive(filename);
    // Opening safely the file
    if (!resultsArchive.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }


    // Retrieve data from txt file
    Data data = Data(instanceFile, 428);

    int numberOfClients = data.getNumberOfClients();
    vector<int> demandOfClients = data.getDemandOfClients();

    vector<Client> clientsInSolution;

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

    auto endTimeHeuristicSolution = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsedTime = endTimeHeuristicSolution - startTimeHeuristicSolution;
    double elapsedSeconds = elapsedTime.count();

    auto startTimeVND = chrono::high_resolution_clock::now();
    heuristicSolution.applyVND(heuristicSolution.solution.travels, fleet, clientsInSolution, formattedOutsourcingCost, data.getMatrixOfCosts());

    auto endTimeVND = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsedTimeVND = endTimeVND - startTimeVND;
    double elapsedSecondsVND = elapsedTimeVND.count();

    resultsArchive << heuristicSolution.solution.total << endl;
    resultsArchive << heuristicSolution.solution.totalCostOfTravels << endl;
    resultsArchive << heuristicSolution.solution.totalCostsFromUsageOfFleet << endl;
    resultsArchive << heuristicSolution.solution.totalCostOfOutsourcing << endl;

    resultsArchive << " " << endl;

    if (heuristicSolution.solution.outsourcedClients.size() > 0) {
        for (int i = 0; i < heuristicSolution.solution.outsourcedClients.size(); i++) {
            resultsArchive << heuristicSolution.solution.outsourcedClients[i].id << " " << endl;
        }
    } else {
        resultsArchive << "nenhum cliente foi terceirizado" << endl;
    }
    

    resultsArchive << " " << endl;

    resultsArchive << heuristicSolution.solution.travels.size() << endl;
    for (Travel& travel : heuristicSolution.solution.travels) {
        for (int i = 0; i < travel.clientsDone.size(); i++) {
            resultsArchive << travel.clientsDone[i].id << " ";
        }
        resultsArchive << " " << endl;
    }

    resultsArchive.close();

    return 0;
}