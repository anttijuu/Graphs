//
//  main.cpp
//  TrainTravelling
//
//  Created by Antti Juustila on 27.11.2020.
//

#include <iostream>
#include <string>
#include <iomanip>

#include "Graph.hpp"
#include "Dijkstra.hpp"
#include "Station.hpp"

void createNetwork(Graph<Station> & network);

int main(int argc, const char * argv[]) {

   std::cout << std::endl << " >>>> Welcome to Finnish railroad network! >>>>" << std::endl << std::endl;

   /// Create the graph using an adjacency list as an implementation.
   Graph<Station> network;

   /// Helper object to easily create Vertexes using just the city name.
   Station helsinki("Helsinki");
   Station tampere("Tampere");
   Station lahti("Lahti");
   Station oulu("Oulu");
   Station vainikkala("Vainikkala");
   Station turku("Turku");

   /// Fill the network with vertexes and edges.
   createNetwork(network);

   /// Print out the network adjacency list.
   std::cout << network << std::endl;

   std::cout << "Is network disconnected: " << (network.isDisconnected() ? "true" : "false") << std::endl;
   std::cout << "Add Pori.." << std::endl;
   auto pori = network.createVertex(Station("Pori", "555-1212", 10, 15));
   std::cout << "Is netword disconnected: " << (network.isDisconnected() ? "true" : "false") << std::endl << std::endl;

   std::cout << "Connecting Pori with Tampere..." << std::endl;
   network.add(EdgeType::EUndirected, pori, tampere, 132);
   std::cout << "Is netword disconnected: " << (network.isDisconnected() ? "true" : "false") << std::endl << std::endl;

   std::cout << network << std::endl;

   std::cout << "Distance from Helsinki to Lahti: " << network.weight(helsinki, lahti) << " km" << std::endl << std::endl;

   // Doesn't work well with undirected graphs, so that's why it is commented out.
   // std::cout << "Number of paths from Oulu to Helsinki: " << network.numberOfPathsFrom(oulu, helsinki) << std::endl << std::endl;

   std::cout << "Breadth first search from " << oulu << ":" << std::endl;
   auto vertices = network.breadthFirstSearchFrom(oulu);
   int counter = 1;
   for (const auto & vertice : vertices) {
      std::cout << std::setw(3) << counter++ << " " << vertice.data.name << " "
         << vertice.data.phone << " " << vertice.data.opens << "-" << vertice.data.closes <<  std::endl;
   }
   std::cout << std::endl;

   std::cout << "Depth first search from " << oulu << ":" << std::endl;
   auto vertices2 = network.depthFirstSearchFrom(oulu);
   counter = 1;
   for (const auto & vertice : vertices2) {
      std::cout << std::setw(3) << counter++ << " " << vertice.data.name << " "
      << vertice.data.phone << " " << vertice.data.opens << "-" << vertice.data.closes <<  std::endl;
   }

   std::cout << std::endl << "Does the network have cycles?: " << (network.hasCycle(pori) ? "yes" : "no") << std::endl << std::endl;

   std::cout << " --- Using Dijkstra's algorithm to find shortest path from Oulu to Vainikkala:" << std::endl << std::endl;
   Dijkstra<Station> dijkstra(network);
   auto pathsFromOulu = dijkstra.shortestPathFrom(oulu);
   auto path = dijkstra.shortestPathTo(vainikkala, pathsFromOulu);
   double total = 0.0;
   std::for_each(path.rbegin(), path.rend(), [&total](const auto & edge) {
      std::cout << std::setw(12) << edge.source << " --> " << std::setw(4) << edge.weight << " -->" << std::setw(12) << edge.destination << std::endl;
      total += edge.weight;
   });
   std::cout << std::setw(17) << ">> Totalling " << std::setw(5) << total << " km" << std::endl << std::endl;

   std::cout << " --- Using Dijkstra's algorithm to find shortest path from Oulu to Turku:" << std::endl << std::endl;
   path = dijkstra.shortestPathTo(turku, pathsFromOulu);
   total = 0.0;
   std::for_each(path.rbegin(), path.rend(), [&total](const auto & edge) {
      std::cout << std::setw(12) << edge.source << " --> " << std::setw(4) << edge.weight << " -->" << std::setw(12) << edge.destination << std::endl;
      total += edge.weight;

   });
   std::cout << std::setw(17) << ">> Totalling " << std::setw(5) << total << " km" << std::endl << std::endl;

   std::cout << std::endl << "<<<< Thank you for travelling in Finland! <<<<" << std::endl << std::endl;
   return EXIT_SUCCESS;
}


void createNetwork(Graph<Station> & network) {
   // See the TrainMap.png of this network.
   auto oulu = network.createVertex(Station("Oulu", "555-1111", 7, 22));
   auto kuopio = network.createVertex(Station("Kuopio", "555-2222", 6, 23));
   auto jyvaskyla = network.createVertex(Station("Jyväskylä", "555-3333", 7, 22));
   auto joensuu = network.createVertex(Station("Joensuu", "555-4444", 9, 19));
   auto tampere = network.createVertex(Station("Tampere", "555-5555", 5, 24));
   auto turku = network.createVertex(Station("Turku", "555-6666", 9, 16));
   auto lahti = network.createVertex(Station("Lahti", "555-7777", 8, 16));
   auto kotka = network.createVertex(Station("Kotka", "555-8888", 8, 22));
   auto vainikkala = network.createVertex(Station("Vainikkala", "555-9999", 4, 24));
   auto helsinki = network.createVertex(Station("Helsinki", "555-1010", 0, 24));

   network.add(EdgeType::EUndirected, oulu, kuopio, 359);
   network.add(EdgeType::EUndirected, oulu, jyvaskyla, 389);
   network.add(EdgeType::EUndirected, oulu, tampere, 495);

   network.add(EdgeType::EUndirected, kuopio, joensuu, 169);
   network.add(EdgeType::EUndirected, kuopio, jyvaskyla, 164);
   network.add(EdgeType::EUndirected, kuopio, lahti, 330);
   network.add(EdgeType::EUndirected, kuopio, kotka, 322);
   network.add(EdgeType::EUndirected, kuopio, vainikkala, 360);

   network.add(EdgeType::EUndirected, jyvaskyla, tampere, 154);
   network.add(EdgeType::EUndirected, jyvaskyla, joensuu, 262);
   network.add(EdgeType::EUndirected, jyvaskyla, kotka, 318);
   network.add(EdgeType::EUndirected, jyvaskyla, lahti, 325);
   network.add(EdgeType::EUndirected, jyvaskyla, vainikkala, 355);

   network.add(EdgeType::EUndirected, joensuu, lahti, 378);
   network.add(EdgeType::EUndirected, joensuu, vainikkala, 289);
   network.add(EdgeType::EUndirected, joensuu, kotka, 369);

   network.add(EdgeType::EUndirected, tampere, turku, 170);
   network.add(EdgeType::EUndirected, tampere, helsinki, 187);
   network.add(EdgeType::EUndirected, tampere, lahti, 175);

   network.add(EdgeType::EUndirected, turku, helsinki, 193);
   network.add(EdgeType::EUndirected, turku, lahti, 265);

   network.add(EdgeType::EUndirected, lahti, vainikkala, 153);
   network.add(EdgeType::EUndirected, lahti, helsinki, 104);
   network.add(EdgeType::EUndirected, lahti, kotka, 115);

   network.add(EdgeType::EUndirected, kotka, vainikkala, 145);
}
