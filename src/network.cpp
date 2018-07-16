#include "network.h"
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

Network::Network(int pathsNumber, int coresNumber) : pathNumber(pathsNumber), coresNumber(coresNumber) {

}

void Network::readGraph(string graphFilepath) {
    int pathBinaryNum = 0;
    ifstream graphFile;
    graphFile.open(graphFilepath, ifstream::in);
    if (graphFile.is_open()) {
        graphFile >> graph.nodesNumber;
        graphFile >> graph.linksNumber;
        graph.edges = new Edge *[graph.nodesNumber];
        for (int i = 0; i < graph.nodesNumber; i++) {
            graph.edges[i] = new Edge[graph.nodesNumber];
        }
        for (int i = 0; i < graph.nodesNumber; i++) {
            for (int j = 0; j < graph.nodesNumber; j++) {
                graph.edges[i][j].cores.resize(coresNumber);
                for (vector<int>::iterator it = graph.edges[i][j].cores.begin();
                     it != graph.edges[i][j].cores.end(); it++) {
                    *it = 0;
                }
                for (int k = 0; k < coresNumber; k++) {
                    graph.edges[i][j].cores[k] = 0;
                }
                graph.edges[i][j].startingNode = i;
                graph.edges[i][j].endingNode = j;
                graphFile >> graph.edges[i][j].length;
                graph.edges[i][j].pathBinaryNumber = -1;

                if (graph.edges[i][j].length > 0) {
                    graph.edges[i][j].exist = 1;
                    graph.edges[i][j].pathBinaryNumber = pathBinaryNum;
                    pathBinaryNum++;
                }
            }
        }
    }
}

void Network::readPaths(string pathsFilepath, string socketsFilepath) {
    ifstream pathsFile, sizesFile;
    pathsFile.open(pathsFilepath, ifstream::in);
    sizesFile.open(socketsFilepath, ifstream::in);
    int start = 0;
    int end = 0;
    int core = 1;
    int numberOfSizes = 20;
    if (pathsFile.is_open() && sizesFile.is_open()) {
        pathsFile >> numberOfPossiblePathsAndSizes;
        sizesFile >> numberOfPossiblePathsAndSizes;
        paths = new int *[numberOfPossiblePathsAndSizes];
        sliceSize = new int *[numberOfPossiblePathsAndSizes];
        for (int i = 0; i < numberOfPossiblePathsAndSizes; i++) {
            paths[i] = new int[numberOfPossiblePathsAndSizes];
            sliceSize[i] = new int[numberOfPossiblePathsAndSizes];
        }
        for (int i = 0; i < numberOfPossiblePathsAndSizes; i++) {
            if (start == end)
                end++;
            for (int j = 0; j < graph.linksNumber + 3; j++) {
                if (j == graph.linksNumber) {
                    if (core == 31) {
                        end++;
                        if (end == 16) {
                            start++;
                            end = 0;
                        }
                        core = 1;
                    }
                    paths[i][j] = core;
                    core++;
                    continue;
                }
                if (j == graph.linksNumber + 1) {
                    paths[i][j] = start;
                    continue;
                }

                if (j == graph.linksNumber + 2) {
                    paths[i][j] = end;
                    continue;
                }
                pathsFile >> paths[i][j];
            }
            for (int l = 0; l < numberOfSizes; l++) {
                sizesFile >> sliceSize[i][l];

            }
        }
    }
}

void Network::readDemands(string demandsFilepath) {
    ifstream demandFile;
    demandFile.open(demandsFilepath, ifstream::in);
    Demand demand;

    if (demandFile.is_open()) {
        demandFile >> demandsNumber;
        demandsList.reserve(demandsNumber);
        while (!demandFile.eof()) {
            demandFile >> demand.sourceNode;
            demandFile >> demand.destinationNode;
            demandFile >> demand.volume;
            demandsList.push_back(demand);
        }
    }
}

void Network::initializeSortedVector() {
    for (int l = 0; l < graph.nodesNumber; l++) {
        sortedDemands.push_back(vector<Demand>());
    }
}

void Network::sortDemands() {
    Network::initializeSortedVector();
    for (vector<Demand>::iterator it = demandsList.begin(); it != demandsList.end(); it++) {
        sortedDemands[it->sourceNode].push_back(it.operator*());
    }
    for (vector<vector<Demand> >::iterator itVector = sortedDemands.begin();
         itVector != sortedDemands.end(); itVector++) {
        sort(itVector.operator*().begin(), itVector.operator*().end());
    }
}

void Network::greedySolve() {
    int start, end, volume, pathTableNumber, slice, edgeNumber = 0, lowest = 0, allocated = 0;
    vector<int>::iterator pos;
    while (!demandsList.empty()) {
        start = demandsList.front().sourceNode;
        end = demandsList.front().destinationNode;
        volume = demandsList.front().volume;

        for (int i = 0; i < numberOfPossiblePathsAndSizes; i++)
        {
            if (paths[i][graph.linksNumber == pathNumber && paths[i][graph.linksNumber + 1] == start && paths[i][graph.linksNumber + 2] == end)
            {
                pathTableNumber = i;
                break;
            }
        }

        for (int i = 0; i < graph.E; i++)
        {
            if (paths[pathTableNumber][i] == 1)
            {
                slice = calculateSlices(pathTableNumber, volume);
                for (int j = 0; j < graph.V; j++)
                {
                    for (int k = 0; k < graph.V; k++)
                    {
                        if (graph.edges[j][k].pathBinaryNumber == i)
                        {
                            pos = min_element(graph.edges[j][k].cores.begin(), graph.edges[j][k].cores.end());
                            *pos += slice;
                            allocated = 1;
                            break;
                        }
                    }
                    if (allocated == 1)
                    {
                        allocated = 0;
                        break;
                    }
                }
            }
        }
        demandList.erase(demandList.begin());
    }
}

int Network::calculateSlices(int point, int volume)
{
    int returnSlices;
    int column = volume / 50;
    returnSlices = sliceSize[point][column];
    return(returnSlices);
}

void Network::printGraph() {
    printf("\n");
    for (int i = 0; i < graph.nodesNumber; i++) {
        for (int j = 0; j < graph.nodesNumber; j++) {
            printf("%2d", graph.edges[i][j].length);
//            if (graph.edges[i][j].pathBinaryNumber != -1)
//                //printf(" %1d ", graph.edges[i][j].pathBinaryNumber);
//            else
            printf("  ");
        }
        printf("\n");
    }
}

void Network::printPaths() {
    for (int i = 0; i < numberOfPossiblePathsAndSizes; i++) {
        for (int j = 0; j < graph.linksNumber; j++) {
            printf("%d ", paths[i][j]);
        }
        printf("\n");
    }
}

void Network::printDemands() {
    for (vector<Demand>::iterator it = demandsList.begin(); it != demandsList.end(); it++) {
        printf("%d %d %d\n", it->sourceNode, it->destinationNode, it->volume);
    }
}

void Network::printSortedDemands() {
    for (vector<vector<Demand> >::iterator itVector = sortedDemands.begin();
         itVector != sortedDemands.end(); itVector++) {
        for (vector<Demand>::iterator it = itVector.operator*().begin(); it != itVector.operator*().end(); it++) {
            printf("%d %d %d\n", it->sourceNode, it->destinationNode, it->volume);
        }
    }
}