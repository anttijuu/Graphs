#ifndef Dijkstra_hpp
#define Dijkstra_hpp

#include <queue>

#include "Graph.hpp"

// Visiting paths are either of type Start (visiting starts) or
// after starting from the first node, travelling between edges.
enum VisitType {
   EStart,
   EEdge
};

// A visit has a type and when visiting between vertices (after the starting node)
// also an edge.
template <typename T>
struct Visit {
   VisitType type;
   Edge<T> edge;
};

// Implementation of the Dijkstra algorithm.
template <typename T>
class Dijkstra {
public:
   // Algorithm is created by giving it an already existing graph to work with.
   Dijkstra(const Graph<T> & graph);

   // Gives the Edges to get to a destination vertex using paths from vertices.
   std::vector<Edge<T>> route(const Vertex<T> & toDestination, const std::map<Vertex<T>, Visit<T>> & paths) const;
   // Gives a distance to destination from given vertex and paths.
   double distance(const Vertex<T> & toDestination, const std::map<Vertex<T>, Visit<T>> & paths) const;
   // Gives a map of visiting paths from a starting vertex.
   std::map<Vertex<T>, Visit<T>> shortestPathsFrom(const Vertex<T> & start) const;
   // Gives an array of edges as the shortest path to a destination using several paths as a starting point.
   std::vector<Edge<T>> shortestPathTo(const Vertex<T> & destination, const std::map<Vertex<T>, Visit<T>> & paths) const;

   // The graph to use to find the shortest path.
   const Graph<T> & graph;
};

// Constructor copies the graph from the parameter to be used by Dijkstra algorithm.
// Original graph is not touched.
template <typename T>
Dijkstra<T>::Dijkstra(const Graph<T> & graph)
: graph(graph) {
}

// Finds a route to a destination using given paths.
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

// Calculates the distance to destination using given paths.
template <typename T>
double Dijkstra<T>::distance(const Vertex<T> & toDestination, const std::map<Vertex<T>, Visit<T>> & paths) const {
   auto path = route(toDestination, paths);
   double totalDistance = 0.0;
   std::for_each(path.begin(), path.end(), [&totalDistance] (const Edge<T> & edge) {
      totalDistance += edge.weight;
   });
   return totalDistance;
}

/*
 A comparator function object.
 This is needed in shortestPathFrom function.
 Without a comparator, priority queue uses the < operator in the Vertex
 to order the elements. In this case, we want the priority queue to order
 the elements based on the _Edge weights_. So we must tell that to the priority queue.

 So the priority queue is used to keep the vertices in order of distance.
 Priority queues in std library allow us to give it a comparator function object
 so that we can tell the priority queue how to order the elements in the queue.
 Usually the implementation is simple, we just compare items in the priority queue
 using some item value or item operator <. But in this case, the Vertex does not
 have any data member we can use in the comparison, but we need the distance from
 or to the vertex using a _path_. This is why we have to give the comparator function
 object additional things to enable it to calculate the distance of paths from
 a vertex. So the comparator is given
 - the Dijkstra object -- so that it can call the distance function, and
 - the path -- since it contains the paths of which the distance is calculated.
 You can see this used in shortestPathFrom function below when it creates the
 std::priority_queue to use. That queue is give this distance_compare function object
 it then uses in keeping the Vertices in the priority queue in order of the distance
 from the vertex via various paths.
 */
template <typename T>
struct distance_compare {
   // A constructor we can use to give the Dijkstra and path objects to the comparator.
   distance_compare(const Dijkstra<T> & d, const std::map<Vertex<T>, Visit<T>> & p)
   : dijkstra(d), paths(p) { /* Empty implementation */ };
   // The actual operator std::priority_queue uses to compare the elements in the queue.
   bool operator () (const Vertex<T> & lhs, const Vertex<T> & rhs) {
      return dijkstra.distance(lhs, paths) < dijkstra.distance(rhs, paths);
   };
   const Dijkstra<T> & dijkstra;                   // Comparator needs this to call distance()
   const std::map<Vertex<T>, Visit<T>> & paths;    // Comparator needs this to calculate the distance.
};

/*
 Function finds the shortest paths from a starting vertex, and returns the paths in a map
 containing the Vertex and the paths as a Visit object (containing the Edges of the path).
 */
template <typename T>
std::map<Vertex<T>, Visit<T>> Dijkstra<T>::shortestPathsFrom(const Vertex<T> & start) const {
   Visit<T> visit;
   visit.type = VisitType::EStart;
   std::map<Vertex<T>, Visit<T>> paths;  // The result returned from this function; shortest paths from start.
   // First insert the starting visiting edge to the path.
   paths.insert( { graph.adjacencies.find(start)->first, visit } );
   //  ↑                            ↑                           ↑
   // Key-value table         The key (Vertex)        The value (Visit)
   
   // Create a priority queue sorting the vertices in the order of path distances from the vertex.
   std::priority_queue<Vertex<T>, std::vector<Vertex<T>>, distance_compare<T>> priorityQueue(distance_compare(*this, paths));
   // When we push vertices in the priority queue, they are then automatically arranged in the order of path distance (length).
   priorityQueue.push(graph.adjacencies.find(start)->first);

   // Start from the starting vertex already in the queue.
   while (!priorityQueue.empty()) {                   // While there are vertices int he queue.
      auto vertex = priorityQueue.top();              // Take the vertice having the shortest distance out.
      priorityQueue.pop();
      auto edges = graph.edges(vertex);               // Get the edges of this vertex
      for (const auto & edge : edges) {               // For each edge...
         double weight = edge.weight;
         // If the edge's destination has not yet been added to paths _or_
         // the distance of the vertex path + current edge's length is _smaller_ than the distance of the
         // edge's destination's paths, then add the edge's destination to the paths as an edge type visit
         // and push the edge's destination to the priority queue.
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
