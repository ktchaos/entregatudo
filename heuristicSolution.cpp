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
    for (int i = 0; i < solution.travels.size(); i++) {
    Travel& travel = solution.travels[i];
        for (int j = 0; j < travel.clientsDone.size() - 1; j++) {
            // cout << "j --------- " << j << endl;
            for (int k = j + 1; k < travel.clientsDone.size(); k++) {
                // Create a new travel by swapping client j and k
                // cout << "k --------- " << k << endl;
                Travel newTravel = travel;
                swap(newTravel.clientsDone[j], newTravel.clientsDone[k]);

                // Calculate the new total cost
                newTravel.totalCost = matrixOfCosts[0][newTravel.clientsDone[0].id]; // cost from depot to first client
                for (int l = 0; l < newTravel.clientsDone.size() - 1; l++) {
                    newTravel.totalCost += matrixOfCosts[newTravel.clientsDone[l].id][newTravel.clientsDone[l + 1].id];
                }
                newTravel.totalCost += matrixOfCosts[newTravel.clientsDone[newTravel.clientsDone.size() - 1].id][0]; // cost from last client to depot

                if (newTravel.totalCost < travel.totalCost) {
                    travel = newTravel;
                }
            }
        }

        // Update total cost of solution
        int aux = 0;
        for (int x = 0; x < solution.travels.size(); x++) {
            aux += solution.travels[x].totalCost;
        }
        solution.totalCostOfTravels = aux;
        solution.total = solution.totalCostOfTravels + solution.totalCostsFromUsageOfFleet + solution.totalCostOfOutsourcing;
    }
}

void HeuristicSolution::apply2OptNeighbor(vector<Travel>& travels, Fleet& fleet, vector<Client>& clients, vector< vector<int> > matrixOfCosts) {
    for (int i = 0; i < solution.travels.size(); i++) {
        Travel& travel = solution.travels[i];
        for (int j = 0; j < travel.clientsDone.size() - 1; j++) {
            for (int k = j + 1; k < travel.clientsDone.size(); k++) {
                // Create a new travel by swapping client j and k
                Travel newTravel = travel;
                swap(newTravel.clientsDone[j], newTravel.clientsDone[k]);

                // Calculate the new total cost
                newTravel.totalCost = matrixOfCosts[0][newTravel.clientsDone[0].id]; // cost from depot to first client
                for (int l = 0; l < newTravel.clientsDone.size() - 1; l++) {
                    newTravel.totalCost += matrixOfCosts[newTravel.clientsDone[l].id][newTravel.clientsDone[l + 1].id];
                }
                newTravel.totalCost += matrixOfCosts[newTravel.clientsDone[newTravel.clientsDone.size() - 1].id][0]; // cost from last client to depot

                // If the new total cost is less than the old total cost, then replace the old travel with the new one
                if (newTravel.totalCost < travel.totalCost) {
                    travel = newTravel;
                }
            }
        }
    }

    int aux = 0;
    for (int x = 0; x < solution.travels.size(); x++) {
        aux += solution.travels[x].totalCost;
    }
    
    solution.totalCostOfTravels = aux;
    solution.total = solution.totalCostOfTravels + solution.totalCostsFromUsageOfFleet + solution.totalCostOfOutsourcing;
}

void HeuristicSolution::applyReinsertionNeighbor(vector<Travel>& travels, Fleet& fleet, vector<Client>& clients, vector< vector<int> > matrixOfCosts) {
    for(int i = 0; i < solution.travels.size(); i++) {
        Travel& travel = solution.travels[i];
        for (int j = 0; j < travel.clientsDone.size(); j++) {
            Client client = travel.clientsDone[j];

            travel.clientsDone.erase(travel.clientsDone.begin() + j);
            travel.totalCost -= matrixOfCosts[client.id][0]; // assuming matrixOfCosts is a global variable

            for(int k = 0; k <= travel.clientsDone.size(); k++) {
                travel.clientsDone.insert(travel.clientsDone.begin() + k, client);

                travel.totalCost = matrixOfCosts[0][travel.clientsDone[0].id]; // cost from depot to first client
                for(int l = 0; l < travel.clientsDone.size() - 1; l++) {
                    travel.totalCost += matrixOfCosts[travel.clientsDone[l].id][travel.clientsDone[l + 1].id];
                }
                travel.totalCost += matrixOfCosts[travel.clientsDone[travel.clientsDone.size() - 1].id][0]; // cost from last client to depot

                travel.clientsDone.erase(travel.clientsDone.begin() + k);
            }
        }
    }

    int aux = 0;
    for (int x = 0; x < solution.travels.size(); x++) {
        aux += solution.travels[x].totalCost;
    }

    solution.totalCostOfTravels = aux;
    solution.total = solution.totalCostOfTravels + solution.totalCostsFromUsageOfFleet + solution.totalCostOfOutsourcing;
}