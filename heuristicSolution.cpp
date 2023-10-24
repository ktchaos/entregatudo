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

void HeuristicSolution::applySingleRouteNeighbor(vector<Travel>& travels, Fleet& fleet, vector<Client>& clients, vector< vector<int> > matrixOfCosts) {
    // void applySingleRouteNeighbor(vector<Travel>& travels, Fleet& fleet, vector<Client>& clients, vector<vector<int>>& matrixOfCosts) {
        for (int i = 0; i < travels.size(); i++) {
            Travel& currentTravel = travels[i];

            // for (int x = 0; x < currentTravel.clientsDone.size(); x++) {
            //     cout << "Client -> " << currentTravel.clientsDone[x].id << endl;
            // }
            
            

            vector<Client>& currentRoute = currentTravel.clientsDone;
            int currentCost = currentTravel.totalCost;
            int currentCapacity = 0;
            
            for (int j = 0; j < currentRoute.size(); j++) {
                currentCapacity += currentRoute[j].demand;
                // cout << "ClientsDone.id = " << currentRoute[j].id << endl;
                // cout << "CurrentCapacity = " << currentCapacity << endl;

                for (int k = 0; k < clients.size(); k++) {
                    if (!clients[k].isOnSolution) {
                        int newCapacity = currentCapacity + clients[k].demand;

                        if (newCapacity <= fleet.capacity) {
                            // cout << "Entrou, newCapactir = " << newCapacity << endl;
                            // Calculate the cost if we insert this client at position j
                            int newCost = currentCost - matrixOfCosts[currentRoute[j - 1].id][currentRoute[j].id];
                            newCost += matrixOfCosts[currentRoute[j - 1].id][clients[k].id];
                            newCost += matrixOfCosts[clients[k].id][currentRoute[j].id];

                            if (newCost < currentTravel.totalCost) {
                                cout << "EH MELHORR, newCapactir = " << newCost << endl;
                                // If the new cost is better, update the route and cost
                                currentRoute.insert(currentRoute.begin() + j, clients[k]);
                                currentTravel.totalCost = newCost;
                                clients[k].isOnSolution = true;
                            }
                        }
                    }
                }

                currentCapacity -= currentRoute[j].demand;
            }

            // // End of route
            // for (int x = 0; x < currentRoute.size(); x++) {
            //     cout << "Client - " << currentRoute[x].id << endl;
            // }
            


        }
    // }
}


void HeuristicSolution::applyMultiRouteNeighbor(vector<Travel>& travels, Fleet& fleet, vector<Client>& clients, vector< vector<int> > matrixOfCosts) {
    for (int i = 0; i < travels.size(); i++) {
        for (int j = i + 1; j < travels.size(); j++) {
            Travel& route1 = travels[i];
            Travel& route2 = travels[j];

            // cout << "id -> " << route1.clientsDone[i].id << endl;

            // Calculate the cost of merging the two routes
            int route1LastClient = route1.clientsDone.back().id;
            int route2FirstClient = route2.clientsDone.front().id;
            int mergeCost = matrixOfCosts[route1LastClient][route2FirstClient];

            if (route1.totalCost + route2.totalCost + mergeCost <= fleet.cost) {
                for (int x = 0; x < route1.clientsDone.size(); x++) {
                    cout << "id -> " << route1.clientsDone[x].id << endl;
                }
                
                cout << "Entrou" << endl;
                // If merging the two routes is feasible and results in a lower cost, merge them
                route1.totalCost += route2.totalCost + mergeCost;
                route1.clientsDone.insert(route1.clientsDone.end(), route2.clientsDone.begin(), route2.clientsDone.end());
                route2.clientsDone.clear();
                travels.erase(travels.begin() + j);
                j--;
            }
        }
    }
}