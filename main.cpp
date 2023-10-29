#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <hpdf.h>
#include "data.h"
#include "heuristicSolution.h"

using namespace std;

float calculateGAP(int value, int optimumValue) {
    double result = ((static_cast<double>(value) - optimumValue) / optimumValue);
    return (result * 100);
}

int main() {
    // Creating file for compute results
    string filename = "results.txt";
    ofstream resultsArchive(filename);
    // Opening safely the file
    if (!resultsArchive.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }


    // Retrieve data from txt file
    Data data1 = Data("instancias/n9k5_A.txt", 428);
    Data data2 = Data("instancias/n9k5_B.txt", 506);
    Data data3 = Data("instancias/n9k5_C.txt", 559);
    Data data4 = Data("instancias/n9k5_D.txt", 408);
    Data data5 = Data("instancias/n14k5_A.txt", 471);
    Data data6 = Data("instancias/n14k5_B.txt", 565);
    Data data7 = Data("instancias/n14k5_C.txt", 569);
    Data data8 = Data("instancias/n14k5_D.txt", 471);
    Data data9 = Data("instancias/n22k3_A.txt", 605);
    Data data10 = Data("instancias/n22k3_B.txt", 777);
    Data data11 = Data("instancias/n22k3_C.txt", 777);
    Data data12 = Data("instancias/n22k3_D.txt", 605);
    Data data13 = Data("instancias/n31k5_A.txt", 650);
    Data data14 = Data("instancias/n31k5_B.txt", 933);
    Data data15 = Data("instancias/n31k5_C.txt", 939);
    Data data16 = Data("instancias/n31k5_D.txt", 656);
    Data data17 = Data("instancias/n43k6_A.txt", 801);
    Data data18 = Data("instancias/n43k6_B.txt", 1203);
    Data data19 = Data("instancias/n43k6_C.txt", 1208);
    Data data20 = Data("instancias/n43k6_D.txt", 802);
    Data data21 = Data("instancias/n64k9_A.txt", 934);
    Data data22 = Data("instancias/n64k9_B.txt", 1503);
    Data data23 = Data("instancias/n64k9_C.txt", 1510);
    Data data24 = Data("instancias/n64k9_D.txt", 932);
    Data data25 = Data("instancias/n120k7_A.txt", 1029);
    Data data26 = Data("instancias/n120k7_B.txt", 2052);
    Data data27 = Data("instancias/n120k7_C.txt", 2040);
    Data data28 = Data("instancias/n120k7_D.txt", 1046);
    Data data29 = Data("instancias/n199k17_A.txt", 1672);
    Data data30 = Data("instancias/n199k17_B.txt", 3302);
    Data data31 = Data("instancias/n199k17_C.txt", 3301);
    Data data32 = Data("instancias/n199k17_D.txt", 1672);

    vector<Data> instances;
    instances.push_back(data1);
    instances.push_back(data2);
    instances.push_back(data3);
    instances.push_back(data4);
    instances.push_back(data5);
    instances.push_back(data6);
    instances.push_back(data7);
    instances.push_back(data8);
    instances.push_back(data9);
    instances.push_back(data10);
    instances.push_back(data11);
    instances.push_back(data12);
    instances.push_back(data13);
    instances.push_back(data14);
    instances.push_back(data15);
    instances.push_back(data16);
    instances.push_back(data17);
    instances.push_back(data18);
    instances.push_back(data19);
    instances.push_back(data20);
    instances.push_back(data21);
    instances.push_back(data22);
    instances.push_back(data23);
    instances.push_back(data24);
    instances.push_back(data25);
    instances.push_back(data26);
    instances.push_back(data27);
    instances.push_back(data28);
    instances.push_back(data29);
    instances.push_back(data30);
    instances.push_back(data31);
    instances.push_back(data32);
    for (int z = 0; z < instances.size(); z++) {
        Data data = instances[z];

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

        resultsArchive << "------------------------------- " << data.getPath() << " -------------------------------" << endl;
        resultsArchive << "Ótimo: "<< data.getOptimumValue() << endl;
        resultsArchive << "----- HEURÍSTICA CONSTRUTIVA -----" << endl;
        resultsArchive << "Valor solução: " << heuristicSolution.solution.total << endl;
        auto endTimeHeuristicSolution = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsedTime = endTimeHeuristicSolution - startTimeHeuristicSolution;
        double elapsedSeconds = elapsedTime.count();
        resultsArchive << "Tempo: " << elapsedSeconds << " segundos" << endl;
        resultsArchive << "gap: " << calculateGAP(heuristicSolution.solution.total, data.getOptimumValue()) << endl;

        auto startTimeVND = chrono::high_resolution_clock::now();
        heuristicSolution.applyVND(heuristicSolution.solution.travels, fleet, clientsInSolution, formattedOutsourcingCost, data.getMatrixOfCosts());
        resultsArchive << "----- VND -----"<< endl;
        resultsArchive << "Valor solução: " << heuristicSolution.solution.total << endl;

        auto endTimeVND = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsedTimeVND = endTimeVND - startTimeVND;
        double elapsedSecondsVND = elapsedTimeVND.count();
        resultsArchive << "Tempo: " << elapsedSecondsVND << " segundos" << endl;
        resultsArchive << "gap: " << calculateGAP(heuristicSolution.solution.total, data.getOptimumValue()) << endl;
        resultsArchive << "-------------------------------------------------------------------------------------" << endl;
    }

    resultsArchive.close();

    return 0;
}