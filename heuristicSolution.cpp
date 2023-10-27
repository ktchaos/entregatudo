#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "heuristicSolution.h"
#include "fleet.h"
#include "client.h"
#include "travel.h"
#include "greedysolution.h"

using namespace std;

HeuristicSolution::HeuristicSolution(Fleet fleet, vector<Client> clients, vector<int> demandOfClients, vector<int> outsourcingCost, vector< vector<int> > matrixOfCosts) {
    /* #### Greedy Algorithm Solution #### */

    vector<Travel> travels;

    vector<int> currentCostLineOnMatrix = matrixOfCosts[0];
    int currentCapacity = 0;
    int aux = 0;
    int numberOfClients = clients.size();

    int numberOfFleetsAvailable = fleet.numberOfVehicles;
    
    Travel travelDone;
    travelDone.totalCost = 0;

    int totalOutsourcingCost = 0;

    while (aux < numberOfClients) {
        aux++;

        // If there is a vehicle available
        if (numberOfFleetsAvailable > 0) {
            vector<double> costOutsourcingRatio;
            // Initialize the costOutsourcingRatio
            for (int i = 0; i < numberOfClients + 1; i++) {
                int currentCost = currentCostLineOnMatrix[i];
                int currentOutsource = outsourcingCost[i];

                float ratio = static_cast<float>(currentCost)/(currentOutsource);
                costOutsourcingRatio.push_back(ratio);
            }

            // Get the lower ratio
            float minCost = 10000;
            int clientNumber = 0;
            for (int i = 0; i < costOutsourcingRatio.size(); i++) {
                float currentCost = costOutsourcingRatio[i];
                if (isnan(currentCost) == true || isnan(minCost) == true
                    || (isinf(currentCost) == true) || (isinf(minCost) == true)
                    || currentCost == 0 || clients[i-1].isOnSolution == true
                    ) {
                    continue;
                }

                if (currentCost < minCost) {
                    minCost = currentCost;
                    clientNumber = i;
                }
            }

            // Check if the fleet supports the demand
            Client choosedClient = clients[clientNumber-1];
            int provisoryCapacity = currentCapacity + choosedClient.demand;

            if (provisoryCapacity < fleet.capacity) {
                cout << "o Client escolhido foi no índice ---> " << clientNumber << endl;
                // Update the current capacity of the fleet
                currentCapacity += choosedClient.demand;
                travelDone.clientsDone.push_back(choosedClient);
                clients[clientNumber-1].isOnSolution = true;
                travelDone.totalCost += currentCostLineOnMatrix[clientNumber];
                // Update the line on the matrix of costs
                currentCostLineOnMatrix = matrixOfCosts[clientNumber];

                // in case it is the last client, finish the travel
                if (aux == numberOfClients) {
                    Client lastClient = travelDone.clientsDone[travelDone.clientsDone.size() - 1];
                    travelDone.totalCost += matrixOfCosts[lastClient.id][0];
                    travels.push_back(travelDone);
                    // END OF THE TRAVEL
                    cout << "FIM DA VIAGEM ---- " << endl;
                    numberOfFleetsAvailable--;
                }
            } else {
                // END OF THE TRAVEL
                cout << "FIM DA VIAGEM ---- " << endl;
                numberOfFleetsAvailable--;
                aux--;
                // Calculates the return to the deposity
                Client lastClient = travelDone.clientsDone[travelDone.clientsDone.size() - 1];
                travelDone.totalCost += matrixOfCosts[lastClient.id][0];
                travels.push_back(travelDone);
                travelDone = Travel();
                travelDone.totalCost = 0;
                // Back to the deposity
                currentCostLineOnMatrix = matrixOfCosts[0];
                currentCapacity = 0;
            }
        } else {
            // In case there are no available vehicles, and it needs to be terceirizado
            vector<Client> clientsRemaining;
            for (int j = 0; j < clients.size(); j++) {
                if (clients[j].isOnSolution == false) {
                    clientsRemaining.push_back(clients[j]);
                }
            }

            Client choosedClient = clientsRemaining[0];
            clients[choosedClient.id-1].isOnSolution = true;
            totalOutsourcingCost += outsourcingCost[choosedClient.id];
        }
    }

    // Build the solution struct
    GreedySolution solution = GreedySolution();
    solution.travels = travels;
    solution.totalCostOfTravels = 0;
    solution.totalCostsFromUsageOfFleet = 0;
    solution.totalCostOfOutsourcing = 0;

    for (int i = 0; i < travels.size(); i++) {
        solution.totalCostOfTravels += travels[i].totalCost;         
    }
    int numberOfVehiclesUsed = fleet.numberOfVehicles - numberOfFleetsAvailable;

    solution.totalCostsFromUsageOfFleet = numberOfVehiclesUsed * fleet.cost;
    solution.totalCostOfOutsourcing = totalOutsourcingCost;
    solution.total = solution.totalCostOfTravels + solution.totalCostsFromUsageOfFleet + solution.totalCostOfOutsourcing;

    this->solution = solution;
}

// MOVIMENTOS DE VIZINHANÇA
// SWAP - APPLY SINGLE ROUTE NEIGHBOR
void HeuristicSolution::applySingleRouteNeighbor(vector<Travel> travels, Fleet fleet, vector<Client> clients, vector< vector<int> > matrixOfCosts) {
    for(int i = 0; i < travels.size(); i++) {
        Travel& currentTravel = travels[i];
        vector<Client> currentRoute = currentTravel.clientsDone;
        int currentCost = currentTravel.totalCost;
        int currentCapacity = 0;
        
        for (int j = 0; j < currentRoute.size(); j++) { 
            if (currentRoute.size() <= 2) {
                // Swap!
                Client temp = currentRoute[0];
                currentRoute[0] = currentRoute[1];
                currentRoute[1] = temp;

                // Calculate the new cost of swapping the clients
                int newCost = matrixOfCosts[0][currentRoute[0].id] + matrixOfCosts[currentRoute[0].id][currentRoute[1].id] + matrixOfCosts[currentRoute[1].id][0];
                if (newCost < currentCost) {
                    // If results in a lower cost, then do it!
                    currentTravel.totalCost = newCost;
                } else {
                    // Else, leave it as it was
                    temp = currentRoute[0];
                    currentRoute[0] = currentRoute[1];
                    currentRoute[1] = temp;
                }
                continue;
            }

            for (int k = j+1; k < currentRoute.size(); k++) {
                Client temp = currentRoute[j];
                currentRoute[j] = currentRoute[k];
                currentRoute[k] = temp;

                int newCost = currentTravel.totalCost - matrixOfCosts[currentRoute[j - 1].id][currentRoute[j].id] - matrixOfCosts[currentRoute[k - 1].id][currentRoute[k].id];
                newCost += matrixOfCosts[currentRoute[j - 1].id][currentRoute[j].id] + matrixOfCosts[currentRoute[j].id][currentRoute[k].id] + matrixOfCosts[currentRoute[k].id][currentRoute[k+1].id];

                if (newCost < currentCost) {
                    // Se a troca resultar em um custo menor, mantenha a troca
                    currentTravel.totalCost = newCost;
                } else {
                    // Caso contrário, desfaça a troca para manter a integridade da rota
                    temp = currentRoute[j];
                    currentRoute[j] = currentRoute[k];
                    currentRoute[k] = temp;
                }
            }
        }
    }
}

void HeuristicSolution::apply2OptNeighbor(vector<Travel>& travels, Fleet& fleet, vector<Client>& clients, vector< vector<int> > matrixOfCosts) {
    for (int i = 0; i < travels.size(); i++) {
        Travel& currentTravel = travels[i];
        vector<Client>& currentRoute = currentTravel.clientsDone;

        // Ajuste para percorrer todos os pares de clientes na rota
        for (int j = 0; j < currentRoute.size() - 1; j++) {
            for (int k = j + 1; k < currentRoute.size() - 1; k++) {
                // Remova os arcos (j, j+1) e (k, k+1) da rota
                int client1 = currentRoute[j].id;
                int client2 = currentRoute[j + 1].id;
                int client3 = currentRoute[k].id;
                int client4 = currentRoute[k + 1].id;

                int costRemoved = matrixOfCosts[client1][client2] + matrixOfCosts[client3][client4];

                // Calcule o custo do novo caminho após a reconexão
                int newCost = matrixOfCosts[client1][client3] + matrixOfCosts[client2][client4];

                if (newCost < costRemoved) {
                    // Se a reconexão resultar em um custo menor, faça a reconexão
                    reverse(currentRoute.begin() + j + 1, currentRoute.begin() + k + 1);
                }
            }
        }
    }

    cout << "2-OPT" << endl;
    for (int x = 0; x < travels.size(); x++) {
        cout << "Rota [" << x << "]" << "  -- " << endl;
        cout << "total cost -> " << travels[x].totalCost << endl;
        for (int y = 0; y < travels[x].clientsDone.size(); y++) {
            cout << "id -> " << travels[x].clientsDone[y].id << endl;
        }
    }
}