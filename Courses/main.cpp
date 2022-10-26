//
//  main.cpp
//  Courses
//
//  Created by Antti Juustila on 27.11.2020.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <clocale>

#include "Graph.hpp"
#include "Dijkstra.hpp"
#include "Course.hpp"

// Helper funcs
void createNetwork(Graph<Course> & network);
void printVertices(const std::vector<Vertex<Course>> & vertices);
void printPath(const std::vector<Edge<Course>> & path);

int main(int argc, const char * argv[]) {

   // Needed to get scandinavian chars aligned properly in printing shortest path.
   std::setlocale(LC_ALL, "fi_FI.UTF-8");
   std::locale loc("fi_FI.UTF-8");
   std::wcout.imbue(loc);

   std::cout << std::endl << " >>>> Welcome to study @ TOL! >>>>" << std::endl;
   std::cout << " *** Compare the results to the LuK-courses.png with this project! *** " << std::endl << std::endl;
   /// Create the graph using an adjacency list as an implementation.
   Graph<Course> network;

   /// Helper object to easily access courses using course code in the demo.
   Course joti("810136P");
   Course lukt("811383A");
   Course kapo("811398A");
   Course ohj1("811104P");

   /// Fill the network with vertices and edges.
   createNetwork(network);

   /// Print out the network adjacency list.
   std::cout << network << std::endl;

   std::cout << "Is network disconnected: " << (network.isDisconnected() ? "true" : "false") << std::endl << std::endl;

   std::cout << "Number of paths from JOTI to LUKT: " << network.numberOfPathsFrom(joti, lukt) << std::endl;
   std::cout << "Number of paths from OHJ1 to KAPO: " << network.numberOfPathsFrom(ohj1, kapo) << std::endl << std::endl;

   std::cout << "Breadth first search from JOTI";
   auto vertices = network.breadthFirstSearchFrom(joti);
   printVertices(vertices);

   std::cout << " --- Depth search examples ---";
   std::cout << "Depth first search from JOTI" << std::endl;
   vertices = network.depthFirstSearchFrom(joti);
   printVertices(vertices);

   std::cout << "Depth first search from OHJ1" << std::endl;
   vertices = network.depthFirstSearchFrom(ohj1);
   printVertices(vertices);

   std::cout << std::endl << "Does the network have cycles?: " << (network.hasCycle(joti) ? "yes" : "no") << std::endl << std::endl;

   // Take all vertices and do depth search for each of them, unless it already is included
   // in some search result.
   std::cout << ">>> All trees in the network: >>>" << std::endl << std::endl;
   vertices = network.allVertices();
   std::vector<Vertex<Course>> visited;
   for (const Vertex<Course> & vertex : vertices) {
      if (std::find(visited.begin(), visited.end(), vertex) == visited.end()) {
         auto depthSearchVertices = network.depthFirstSearchFrom(vertex);
         printVertices(depthSearchVertices);
         for (auto vertex : depthSearchVertices) {
            visited.push_back(vertex);
         }
      }
   }
   std::cout << "<<< End of all trees in the network: <<<" << std::endl << std::endl;

   std::cout << " --- Using Dijkstra's algorithm to find shortest path from JOTI to KAPO:" << std::endl << std::endl;
   Dijkstra<Course> dijkstra(network);
   auto pathsFromJOTI = dijkstra.shortestPathsFrom(joti);
   auto path = dijkstra.shortestPathTo(kapo, pathsFromJOTI);
   printPath(path);

   std::cout << " --- Using Dijkstra's algorithm to find shortest path from OHJ1 to KAPO:" << std::endl << std::endl;
   auto pathsFromOHJ1 = dijkstra.shortestPathsFrom(ohj1);
   path = dijkstra.shortestPathTo(kapo, pathsFromOHJ1);
   printPath(path);


   auto topologicalList = network.topologicalSort();
   std::cout << std::endl << " --- Topological sort list of courses: " << std::endl;
   if (topologicalList.empty()) {
      std::cout << std::endl << "    Not able to sort, maybe graph is not directed & acyclic? " << std::endl;
   }
   printVertices(topologicalList);

   std::cout << std::endl << "<<<< Thank you for studying @ TOL! <<<<" << std::endl << std::endl;
   return EXIT_SUCCESS;
}


void createNetwork(Graph<Course> & network) {
   // See the LuK-courses.png of this network.
   auto tiha = network.createVertex(Course("030005P", "TIHA", L"Tiedonhankintakurssi", 3, 1));
   auto suur = network.createVertex(Course("900105Y", "SUUR", L"Suuntaa uralle viestinnän keinoin", 3, 2));
   auto orop = network.createVertex(Course("810020Y", "OROP", L"Orientoivat opinnot", 1, 1));
   auto joti = network.createVertex(Course("810136P", "JOTI", L"Johdatus tietojenkäsittelytieteisiin", 1, 1));
   auto titu = network.createVertex(Course("811168P", "TITU", L"Tietoturva", 1, 3));
   auto ohli = network.createVertex(Course("811174P", "OHLP", L"Ohjelmistoliiketoiminnan perusteet", 1, 3));
   auto lati = network.createVertex(Course("811102P", "LATI", L"Laitteet ja tietoverkot", 1, 1));

   auto ohj1 = network.createVertex(Course("811104P", "OHJ1", L"Ohjelmointi 1", 1, 1));
   auto ohj2 = network.createVertex(Course("811322A", "OHJ2", L"Ohjelmointi 2", 1, 3));
   auto tika = network.createVertex(Course("811325A", "TIKA", L"Tietokannat", 1, 1));
   auto tira = network.createVertex(Course("811312A", "TIRA", L"Tietorakenteet ja algoritmit", 2, 2));
   auto ohj3 = network.createVertex(Course("811367A", "OHJ3", L"Ohjelmointi 3", 2, 3));
   auto ohj4 = network.createVertex(Course("811368A", "OHJ4", L"Ohjelmointi 4 ", 2, 4));

   auto joht = network.createVertex(Course("811103P", "JOHT", L"Johdatus ohjelmistotuotantoon", 1, 2));
   auto vama = network.createVertex(Course("811391A", "VAMA", L"Vaatimusmäärittely", 1, 4));
   auto ohms = network.createVertex(Course("811301A", "OHMS", L"Ohjelmistojen mallinnus ja suunnittelu", 2, 1));
   auto late = network.createVertex(Course("811306A", "OLAT", L"Ohjelmistojen laatu ja testaus", 2, 2));
   auto tims = network.createVertex(Course("811319A", "TIMS", L"Tietomallinnus ja -suunnittelu", 2, 3));
   auto ohar = network.createVertex(Course("815345A", "OHAR", L"Ohjelmistoarkkitehtuurit", 2, 4));

   auto tjps = network.createVertex(Course("811166P", "TJPE", L"Tietojärjestelmien perusteet", 1, 2));
   auto jmsk = network.createVertex(Course("812360A", "TMSK", L"Tietojärjestelmien mallintaminen, suunnittelu ja kehitys", 1, 4));
   auto ihsu = network.createVertex(Course("812363A", "IHSU", L"Ihmislähtöinen suunnittelu", 2, 1));
   auto tjhk = network.createVertex(Course("812361A", "TJHK", L"Tietojärjestelmien hankinta, käyttöönotto ja hallinta", 2, 2));
   auto lpjm = network.createVertex(Course("812362A", "LPMJ", L"Liiketoimintaprosessien johtaminen ja mallintaminen", 2, 3));
   auto data = network.createVertex(Course("812364A", "DATA", L"Data-analytiikka liiketoiminnan tukena", 3, 4));

   auto prot = network.createVertex(Course("811397A", "PROT", L"Projektitoiminnan perusteet", 2, 4));
   auto kapo = network.createVertex(Course("811398A", "KAPO", L"Kandidaattiprojekti", 3, 3));

   auto jotu = network.createVertex(Course("811393A", "JOTU", L"Johdatus tutkimustyöhön", 2, 3));
   auto lukt = network.createVertex(Course("811383A", "LUKT", L"LuK-tutkielma", 3, 3));

   // Intro module
   network.add(EdgeType::EDirected, joti, titu, 1);
   network.add(EdgeType::EDirected, joti, ohli, 1);
   network.add(EdgeType::EDirected, joti, joht, 1);
   network.add(EdgeType::EDirected, joti, lukt, 1);
   network.add(EdgeType::EDirected, lati, titu, 1);

   // SE module
   network.add(EdgeType::EDirected, joht, vama, 1);
   network.add(EdgeType::EDirected, vama, ohms, 1);
   network.add(EdgeType::EDirected, ohms, late, 1);
   network.add(EdgeType::EDirected, late, tims, 1);
   network.add(EdgeType::EDirected, tims, ohar, 1);
   network.add(EdgeType::EDirected, ohar, kapo, 1);

   // Programming module
   network.add(EdgeType::EDirected, ohj1, ohj2, 1);
   network.add(EdgeType::EDirected, ohj1, tika, 1);
   network.add(EdgeType::EDirected, ohj2, ohms, 1);
   network.add(EdgeType::EDirected, ohj2, tira, 1);
   network.add(EdgeType::EDirected, tika, tims, 1);
   network.add(EdgeType::EDirected, ohj2, ohj3, 1);
   network.add(EdgeType::EDirected, ohj3, ohj4, 1);
   network.add(EdgeType::EDirected, ohj4, kapo, 1);

   // IS module
   network.add(EdgeType::EDirected, tjps, jmsk, 1);
   network.add(EdgeType::EDirected, jmsk, ihsu, 1);
   network.add(EdgeType::EDirected, ihsu, tjhk, 1);
   network.add(EdgeType::EDirected, tjhk, lpjm, 1);
   network.add(EdgeType::EDirected, lpjm, data, 1);
   network.add(EdgeType::EDirected, lpjm, kapo, 1);

   // Capstone module
   network.add(EdgeType::EDirected, prot, kapo, 1);
   network.add(EdgeType::EDirected, tiha, jotu, 1);
   network.add(EdgeType::EDirected, suur, jotu, 1);
   network.add(EdgeType::EDirected, tiha, lukt, 1);
   network.add(EdgeType::EDirected, jotu, lukt, 1);

   // Testing with better dependencies; related to topological sort
   // Uncomment these to check out topological sorting with decent result!
   // network.add(EdgeType::EDirected, joti, tjps, 1);
   // network.add(EdgeType::EDirected, lati, ohj1, 1);
   // network.add(EdgeType::EDirected, titu, tika, 1);
   // network.add(EdgeType::EDirected, late, prot, 1);
}

void printVertices(const std::vector<Vertex<Course>> & vertices) {
   int counter = 1;
   for (const auto & vertice : vertices) {
      std::cout << std::setw(3) << counter++ << " " << vertice.data.code << " ";
      std::wcout << vertice.data.name << " " << vertice.data.year << "-" << vertice.data.period <<  std::endl;
   }
   std::cout << std::endl;
}

void printPath(const std::vector<Edge<Course>> & path) {
   // Path has the elements in opposite order, so printing out them in reverse order using rbegin/rend.
   double total = 0.0;
   std::for_each(path.rbegin(), path.rend(), [&total](const auto & edge) {
      std::wcout << std::right << std::setw(40) << edge.source.data.name << std::setw(6) << " --> ";
      std::wcout << std::left << std::setw(40) << edge.destination.data.name << std::endl;
      total += edge.weight;
   });
   std::cout << std::setw(40) << ">> Totalling " << std::setw(5) << total << " steps" << std::endl << std::endl;
}
