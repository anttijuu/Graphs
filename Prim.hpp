#ifndef Prim_hpp
#define Prim_hpp

#include <queue>
#include <utility> // for make_pair

#include "Graph.hpp"

/**
 Prim algorithm finds us the minimum spanning three for a graph.
 See the output of the algorithm in the examples, and compare the
 output to the image files drawn based on the output to see what thee
 algorithm actually does, especially in the TrainTravelling example.
 */

template <typename T>
class Prim {
public:
   Prim();

   // A helper function used by the produceMinimumSpanningTreeFor().
   void addAvailableEdges(const Vertex<T> & vertex,
                          const Graph<T> & graph,
                          const std::set<Vertex<T>> & visited,
                          std::priority_queue<Edge<T>, std::vector<Edge<T>>, weight_compare<T>> & toQueue);

   // The work of Prim algorithm is done in this function, for a graph given as parameter.
   std::pair<double,Graph<T>> produceMinimumSpanningTreeFor(const Graph<T> & graph);
};

// Nothing to do here.
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
   // Contains the new graph with minimum spanning tree, returned form the function.
   Graph<T> minimumSpanningTree;
   // To keep track where we have been.
   std::set<Vertex<T>> visited;
   // A priority queue, using the weight_compare function object to sort the Edges in the
   // queue in thee order of the edge _weights_. Otherwise, the Edge operator < would be used
   // in determining the priorities. weight_compare is declared in Edge.hpp.
   // Note that weight_compare is: lhs.weight > rhs.weight -- this is because the std::priority_queue
   // is a _max_ priority queue but we here need a _min_ priority queue. So we ask the queue to
   // order the elements in greater-than order so when we call queue.top(), we get the _smallest_, not
   // largest elements out of the queue first.
   std::priority_queue<Edge<T>, std::vector<Edge<T>>, weight_compare<T>> priorityQueue;  // TODO: check out the sorting of the queue

   minimumSpanningTree.copyVertices(graph);     // Copy vertices from the original graph, no edges.

   // If the graph has nothing in, return cost of 0.0 and the empty minimum spanning tree.
   if (graph.adjacencies.empty()) {
      return std::make_pair(cost, minimumSpanningTree);
   }
   // Start handling the graph from the first element.
   Vertex<T> start = graph.adjacencies.begin()->first;
   visited.insert(start);
   // Add the available edges from the starting vertex to the priority queue from the graph
   // if it has not been visited before.
   addAvailableEdges(start, graph, visited, priorityQueue);
   // While there are things in the queue...
   while (!priorityQueue.empty()) {
      // Take the edge with smallest distance...
      const auto smallestEdge = priorityQueue.top();
      priorityQueue.pop();
      // ...check the destination and if we have visited it, take the next item from the queue.
      const auto vertex = smallestEdge.destination;
      if (visited.find(vertex) != visited.end()) {
         continue;
      }
      // Otherwise, insert the destination to the visited set.
      visited.insert(vertex);
      // Add the weight of the edge to total cost.
      cost += smallestEdge.weight;
      // And add tthe edge's source and destination with the weight to the minimum spanning tree...
      minimumSpanningTree.add(EdgeType::EUndirected, smallestEdge.source, smallestEdge.destination, smallestEdge.weight);
      // ...and add the available edges from the starting vertex to the priority queue from the graph
      // if it has not been visited before.
      addAvailableEdges(vertex, graph, visited, priorityQueue);
   }
   // Finished, so take the total cost of the minimum spanning tree and the tree itself,
   // make a pair out of them and return the pair to the caller.
   return std::make_pair(cost, minimumSpanningTree);
}

#endif
