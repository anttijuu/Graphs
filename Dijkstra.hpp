#ifndef Dijkstra_hpp
#define Dijkstra_hpp

#include <queue>

#include "Graph.hpp"

enum VisitType {
   EStart,
   EEdge
};

template <typename T>
struct Visit {
   VisitType type;
   Edge<T> edge;
};

template <typename T>
class Dijkstra {
public:
   Dijkstra(const Graph<T> & graph);

   std::vector<Edge<T>> route(const Vertex<T> & toDestination, const std::map<Vertex<T>, Visit<T>> & paths) const;
   double distance(const Vertex<T> & toDestination, const std::map<Vertex<T>, Visit<T>> & paths) const;
   std::map<Vertex<T>, Visit<T>> shortestPathFrom(const Vertex<T> & start) const;
   std::vector<Edge<T>> shortestPathTo(const Vertex<T> & destination, const std::map<Vertex<T>, Visit<T>> & paths) const;

   Graph<T> graph;
};


template <typename T>
Dijkstra<T>::Dijkstra(const Graph<T> & graph) {
   this->graph = graph;
}

template <typename T>
std::vector<Edge<T>> Dijkstra<T>::route(const Vertex<T> & toDestination, const std::map<Vertex<T>, Visit<T>> & paths) const {
   auto vertex = toDestination;
   std::vector<Edge<T>> path;

   if (paths.size() == 0) {
      return path;
   }
   Visit visit = paths.at(vertex);
   while (visit.type != VisitType::EStart) {
      path.push_back(visit.edge);
      vertex = visit.edge.source;
      visit = paths.at(vertex); 
   }
   return path;
}

template <typename T>
double Dijkstra<T>::distance(const Vertex<T> & toDestination, const std::map<Vertex<T>, Visit<T>> & paths) const {
   auto path = route(toDestination, paths);
   double totalDistance = 0.0;
   std::for_each(path.begin(), path.end(), [&totalDistance] (const Edge<T> & edge) {
      totalDistance += edge.weight;
   });
   return totalDistance;
}

template <typename T>
std::map<Vertex<T>, Visit<T>> Dijkstra<T>::shortestPathFrom(const Vertex<T> & start) const {
   Visit<T> visit;
   visit.type = VisitType::EStart;
   std::map<Vertex<T>, Visit<T>> paths;
   paths.insert(std::pair<Vertex<T>,Visit<T>>(start, visit));

   std::priority_queue<Vertex<T>> priorityQueue;
   priorityQueue.push(start);

   while (!priorityQueue.empty()) {
      auto vertex = priorityQueue.top();
      priorityQueue.pop();
      auto edges = graph.edges(vertex);
      for (const auto & edge : edges) {
         double weight = edge.weight;
         if (paths.find(edge.destination) == paths.end() ||
             distance(vertex, paths) + weight < distance(edge.destination, paths)) {
            paths[edge.destination].type = VisitType::EEdge;
            paths[edge.destination].edge = edge;
            priorityQueue.push(edge.destination);
         }
      }
   }
   return paths;
}

template <typename T>
std::vector<Edge<T>> Dijkstra<T>::shortestPathTo(const Vertex<T> & destination, const std::map<Vertex<T>, Visit<T>> & paths) const {
   return route(destination, paths);
}

#endif
