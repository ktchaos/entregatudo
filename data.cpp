#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include "data.h"
#include <vector>

using namespace std;

// Constructor:

Data::Data(string path) {
    ifstream myFile(path);

    int aux;

    if (myFile.is_open()) {
        while (myFile.good()) {
            myFile >> numberOfClients;
            myFile >> numberOfVehicles;
            myFile >> capacityOfVehicle;
            myFile >> minOfDeliveriesToBeDone;
            myFile >> costOfVehicle;

            for(int i = 0; i < numberOfClients; i++) {
                myFile >> aux;
                demandOfClients.push_back(aux);
            }

            for(int i = 0; i < numberOfClients; i++) {
                myFile >> aux;
                outsourcingCost.push_back(aux);
            }

            vector<int> line;

            for(int i = 0; i < numberOfClients + 1; i++) {
                for(int j = 0; j < numberOfClients + 1; j++) {
                    myFile >> aux;
                    line.push_back(aux);
                }
                matrixOfCosts.push_back(line);
                line.clear();
            }
            break;
        }
    } else cout << "Unable to open file in path" << path;
}

// Methods:

void Data::printNumberOfClients() {
    cout << numberOfClients << endl;
}

void Data::printNumberOfVehicles() {
    cout << numberOfVehicles << endl;
}

void Data::printCapacityOfVehicle() {
    cout << capacityOfVehicle << endl;
}

void Data::printMinOfDeliveriesToBeDone() {
    cout << minOfDeliveriesToBeDone << endl;
}

void Data::printCostOfVehicle() {
    cout << costOfVehicle << endl;
}

void Data::printDemandOfClients() {
    print(demandOfClients);
}

void Data::printOutsourcingCost() {
    print(outsourcingCost);
}

void Data::printMatrixOfCosts() {
    printMatrix(matrixOfCosts);
}

void Data::print(std::vector<int> const &input) {
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ' ';
    }
}

void Data::printMatrix(const vector< vector<int> >& matrix) {
    int numLines = matrix.size();
    if (numLines == 0) {
        cout << "Error: Matrix is empty" << endl;
        return;
    }

    int numCols = matrix[0].size();
    for (int i = 0; i < numLines; i++) {
        for (int j = 0; j < numCols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}