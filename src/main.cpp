#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include "network.h"

using namespace std;

int main() {
    int pathsNumber = 30;
    int coresNumber = 7;

    string graphPath = "/mnt/d/projekty/rssa-simulator/Euro16/ee.net";
    string demandsPath = "/mnt/d/projekty/rssa-simulator/Euro16/01.dem";
    string pathsPath = "/mnt/d/projekty/rssa-simulator/Euro16/e30.pat";
    string socketsPath = "/mnt/d/projekty/rssa-simulator/Euro16/e30.spec";

    Network Network(pathsNumber, coresNumber);
    Network.readGraph(graphPath);
    Network.readPaths(pathsPath, socketsPath);
    Network.readDemands(demandsPath);
    Network.sortDemands();

   // Network.printGraph();
//    cout << endl << "//////////////////////////////////////////////" << endl;
//    Network.printPaths();
//    cout << endl << "//////////////////////////////////////////////" << endl;
//    Network.printDemands();
   // Network.printSortedDemands();

    return 0;
}