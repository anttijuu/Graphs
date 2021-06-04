//
//  main.cpp
//  TrainTravelling
//
//  Created by Antti Juustila on 27.11.2020.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <clocale>
#include <fstream>
#include <sstream>

#include "Graph.hpp"
#include "Dijkstra.hpp"
#include "Prim.hpp"

/* C++/STL implementation of the course exercise work project. The
   goal is to find a path in a network of cities where the height of
   the road is the lowest compared to other routes.

   The implementation is not the fastest one and sometimes does not find
   the expected path, but another one.
 */

// Helper funcs
bool createNetwork(const std::string & filePath, Graph<int> & network);
void printVertices(const std::vector<Vertex<int>> & vertices);
void printPath(const std::vector<Edge<int>> & path);
void printPaths(const std::map<Vertex<int>, Visit<int>> & paths);

int targetCity = 0;
int startCity = 1;

int main(int argc, const char * argv[]) {

   if (argc != 2) {
      std::cout << "Must give the route file name as parameter!" << std::endl;
      std::cout << "Example: " << std::endl;
      std::cout << "lowestpath path_to_file/graph_ADS2018_20.txt" << std::endl;
      return EXIT_FAILURE;
   }
   std::cout << std::endl << " >>>> Welcome to find the lowest route between cities! >>>>" << std::endl;
   /// Create the graph using an adjacency list as an implementation.

   std::chrono::system_clock::time_point started = std::chrono::system_clock::now();

   Graph<int> network;

   /// Fill the network with vertices and edges.
   const std::string filePath = std::string(argv[1]);
   std::cout << " --- Using file " << filePath << std::endl;
   if (!createNetwork(filePath, network)) {
      std::cout << "Could not read the data file!" << std::endl;
      return EXIT_FAILURE;
   }

   std::cout << " --- Using Dijkstra's algorithm to path with lowest height between cities..." << std::endl << std::endl;
   Dijkstra<int> dijkstra(network);
   auto pathsFromStart = dijkstra.lowestPathsFrom(Vertex<int>(startCity));
   auto path = dijkstra.shortestPathTo(Vertex<int>(targetCity), pathsFromStart);
   printPath(path);

   std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
   std::chrono::milliseconds timeValue = std::chrono::duration_cast<std::chrono::milliseconds>(now-started);
   std::cout << " Finding the route took " << timeValue.count() << " ms" << std::endl;

   std::cout << std::endl << "<<<< Thank you for using lowestroute! <<<<" << std::endl << std::endl;
   return EXIT_SUCCESS;
}


bool createNetwork(const std::string & filePath, Graph<int> & network) {
   bool done = false;
   try {
      std::ifstream file(filePath);
      int currentLine = 0;
      int linesToRead = 0;
      std::string line;
      int inLine = 0;
      int citiesInFile = 0;
      int edgesInFile = 0;
      while (std::getline(file, line))
      {
         std::istringstream iss(line);
         if (inLine == 0) {
            if (!(iss >> citiesInFile >> edgesInFile)) {
               break;
            }
            inLine++;
         } else if (inLine <= edgesInFile) {
            int city1 = 0;
            int city2 = 0;
            int weight = 0;
            if (!(iss >> city1 >> city2 >> weight)) {
               break;
            }
            auto vCity1 = network.createVertex(city1);
            auto vCity2 = network.createVertex(city2);
            network.addUndirectedEdge(vCity1, vCity2, weight);
            inLine++;
         } else {
            // last line
            if (!(iss >> targetCity)) {
               break;
            }
            done = true;
         }
      }
      file.close();
   } catch (std::exception & e) {
      std::cout << "Could not open file: " << e.what() << std::endl;
      return false;
   }
   return done;
}

void printVertices(const std::vector<Vertex<int>> & vertices) {
   int counter = 1;
   for (const auto & vertice : vertices) {
      std::cout << std::setw(3) << counter++ << " " << vertice.data << " ";
   }
   std::cout << std::endl;
}

void printPath(const std::vector<Edge<int>> & path) {
   // Path has the elements in opposite order, so printing out them in reverse order using rbegin/rend.
   double total = 0.0;
   double max = -1.0;
   std::for_each(path.rbegin(), path.rend(), [&total, &max](const auto & edge) {
      std::wcout << std::right << std::setw(10) << edge.source.data << std::setw(6) << " --> ";
      std::wcout << std::left << std::setw(10) << edge.destination.data;
      std::wcout << " : " << edge.weight << "m" << std::endl;
      total += edge.weight;
      if (edge.weight > max) {
         max = edge.weight;
      }
   });
   std::cout << std::setw(20) << ">> Max height: " << std::setw(5) << max << " metres" << std::endl;
   std::cout << std::setw(20) << ">> Totalling: " << std::setw(5) << total << " metres" << std::endl << std::endl;
}

void printPaths(const std::map<Vertex<int>, Visit<int>> & paths) {
   // Visits arranged by city operator < so based on city name.
   // We wish to print starting visit first, then other so let's do that.
   // Using tempPath since it is easier to erase paths as we print them and
   // concentrate on those paths not yet printed.
   auto tempPath = paths;

   // Printing out paths in some sensible order until all are handled and tempPath is empty.
   Vertex<int> city;
   std::queue<Vertex<int>> nextcities;
   while (!tempPath.empty()) {
      bool startFound = false;
      auto iterator = tempPath.begin();
      // First print the starting edge, and remove it from map when done.
      while (!startFound && iterator != tempPath.end()) {
         if (iterator->second.type == VisitType::EStart) {
            city = iterator->first;
            std::cout << "Starting from: " << city << std::endl;
            tempPath.erase(iterator);
            nextcities.push(city);
            startFound = true;
            break; // (iterator != paths.end())
         }
         iterator++;
      }
      // Then print edges in order and remove them when done. Only Edges here so no need to check visit type.
      while (!nextcities.empty()) {
         city = nextcities.front();
         nextcities.pop();
         iterator = tempPath.begin();
         while (iterator != tempPath.end()) {
            if (iterator->second.edge.source == city) {
               std::cout << " Edge " << iterator->second.edge.source << "-> " << iterator->second.edge.destination;
               std::cout << " " << iterator->second.edge.weight << " km." << std::endl;
               nextcities.push(iterator->second.edge.destination);
               tempPath.erase(iterator);
               iterator = tempPath.begin();
            } else {
               iterator++;
            }
         }
      }
   }
   std::cout << std::endl;
}
