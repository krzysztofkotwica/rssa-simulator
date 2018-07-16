#pragma once

#include <vector>
#include <string>

using namespace std;

class Network {
    struct Edge {
        int startingNode;
        int endingNode;
        int length;
        vector<int> cores;
        int maxSlice;
        int exist;
        int pathBinaryNumber;
    };

    struct Graph {
        int nodesNumber; //V
        int linksNumber; //E
        int coresNumber;
        int demandsNumber;
        Edge **edges;
    };

    struct Demand {
        int sourceNode;
        int destinationNode;
        int volume;

        bool operator < (const Demand &demand) const {
            return destinationNode < demand.destinationNode;
        }

    };

    int demandsNumber;
    vector<Demand> demandsList;
    vector< vector<Demand> > sortedDemands;
    Graph graph;
    int pathAndSlicesSize;
    int **paths;
    int numberOfPossiblePathsAndSizes;
    int sliceSizes;
    int **sliceSize;
    int pathNumber;
    int coresNumber;

public:
    Network(int pathsNumber, int coresNumber);

    void readGraph(string graphFilepath);

    void readDemands(string demandsFilepath);

    void readPaths(string pathsFilepath, string socketsFilepath);

    void initializeSortedVector();

    void sortDemands();

    void greedySolve();

    int calculateSlices(int point, int volume);

    void printPaths();

    void printDemands();

    void printGraph();

    void printSortedDemands();
};

