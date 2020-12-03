#ifndef Prim_hpp
#define Prim_hpp

#include <queue>
#include <utility> // for make_pair

#include "Graph.hpp"

template <typename T>
struct weight_compare {
   bool operator () (const Edge<T> & lhs, const Edge<T> & rhs) {
      return lhs.weight > rhs.weight;
   };
};


template <typename T>
class Prim {
public:
   Prim();

   void addAvailableEdges(const Vertex<T> & vertex,
                          const Graph<T> & graph,
                          const std::set<Vertex<T>> & visited,
                          std::priority_queue<Edge<T>, std::vector<Edge<T>>, weight_compare<T>> & toQueue);

   std::pair<double,Graph<T>> produceMinimumSpanningTreeFor(const Graph<T> & graph);
};


template <typename T>
Prim<T>::Prim() {
}


/*
 Utility func called by the produceMinimumSpanningTreeFor().
 */
template <typename T>
void Prim<T>::addAvailableEdges(const Vertex<T> & vertex,
                       const Graph<T> & graph,
                       const std::set<Vertex<T>> & visited,
                       std::priority_queue<Edge<T>, std::vector<Edge<T>>, weight_compare<T>> & toQueue) {

   for (const Edge<T> & edge : graph.edges(vertex)) {
      if (visited.find(edge.destination) == visited.end()) {
         toQueue.push(edge);
      }
   }
}


/*
 Produces a minimum spanning tree of the graph given as a parameter.
 New minimum spanning tree is returned as the second element of the pair.
 First element of the returned pair contains the summed weight of the edges
 in the minimum spanning tree.
 */
template <typename T>
std::pair<double,Graph<T>> Prim<T>::produceMinimumSpanningTreeFor(const Graph<T> & graph) {
   double cost = 0.0;
   Graph<T> minimumSpanningTree;                // Contains the new graph with minimum spanning tree.
   std::set<Vertex<T>> visited;                 // To keep track where we have been.
   std::priority_queue<Edge<T>, std::vector<Edge<T>>, weight_compare<T>> priorityQueue;  // TODO: check out the sorting of the queue

   minimumSpanningTree.copyVertices(graph);     // Copy vertices from the original graph, no edges.

   if (graph.adjacencies.empty()) {
      return std::make_pair(cost, minimumSpanningTree);
   }
   Vertex<T> start = graph.adjacencies.begin()->first;
   visited.insert(start);
   addAvailableEdges(start, graph, visited, priorityQueue);
   while (!priorityQueue.empty()) {
      const auto smallestEdge = priorityQueue.top();
      priorityQueue.pop();
      const auto vertex = smallestEdge.destination;
      if (visited.find(vertex) != visited.end()) {
         continue;
      }
      visited.insert(vertex);
      cost += smallestEdge.weight;
      minimumSpanningTree.add(EdgeType::EUndirected, smallestEdge.source, smallestEdge.destination, smallestEdge.weight);
      addAvailableEdges(vertex, graph, visited, priorityQueue);
   }
   return std::make_pair(cost, minimumSpanningTree);
}

#endif
