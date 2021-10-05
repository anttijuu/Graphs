//
//  Graph.hpp
//  TrainTravelling
//
//  Created by Antti Juustila on 27.11.2020.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <iostream>  // input/output
#include <map>       // associative containers
#include <vector>    // arrays
#include <queue>     // basic queue used in different algorithms
#include <set>       // unordered collection, helper container used in some algos
#include <stack>     // basic stack, used in many places when going through the graph.

#include "Vertex.hpp"   // a node for the graph
#include "Edge.hpp"     // an edge of the graph, from node to another

enum EdgeType {
   EDirected,        // Edge is directed, one way only
   EUndirected       // Edge is two ways, from A to B and then back from B to A
};


/// Graph implemented as an edge type adjacency list.
/// Each Vertex (node) has edge(s) to adjacent other vertices.
/// Edges can be directed (one direction only) or undirected (towards both directions).
/// Each Edge has a weight attribute, a number that can be used to "weight" the edge.
/// For example, it could be a distance (in km) or price or time to travel that edge.
/// T can be any type that can be copied, assigned, compared (==, <) and printed with <<.
template <typename T>
class Graph {
public:

   /// Creates a vertex with no edges using given data.
   Vertex<T> createVertex(const T & data);

   /// Add an edge with type (directional, undirectional) from source to destination (and possibly back).
   void add(EdgeType edge, const Vertex<T> & source, const Vertex<T> & destination, double weight);
   /// Add an undirected edge from source to destination vertex, using a weight.
   void addUndirectedEdge(const Vertex<T> & source, const Vertex<T> & destination, double weight);
   /// Add a directed edge from source to destination vertex.
   void addDirectedEdge(const Vertex<T> & source, const Vertex<T> & destination, double weight);

   void copyVerticesFrom(const Graph<T> & graph);

   /// Get all the edges from a vertex.
   const std::vector<Edge<T>> & edges(const Vertex<T> & fromSource) const;
   /// Gets the weight of the edge from source to destination.
   /// Returns zero if there is no edge from source to destination.
   double weight(const Vertex<T> & fromSource, const Vertex<T> & toDestination) const;

   /// Does a breadth first search from vertex through the graph.
   std::vector<Vertex<T>> breadthFirstSearchFrom(const Vertex<T> & from) const;
   /// Does a depth first search from vertex through the graph.
   std::vector<Vertex<T>> depthFirstSearchFrom(const Vertex<T> & from) const;

   /// Gets all the vertices of the graph.
   std::vector<Vertex<T>> allVertices() const;

   /// Gets the number of paths from a vertex to another.
   int numberOfPathsFrom(const Vertex<T> & source, const Vertex<T> & destination) const;

   /// Checks if the graph contains areas that are disconnected.
   bool isDisconnected() const;
   /// Checks if the graph contains cycles between the vertices.
   bool hasCycle(const Vertex<T> & source) const;

   /// Topological sort with DFS
   std::vector<Vertex<T>> topologicalSort() const;

   /// Associative table; a dictionary where a Vertex has an array of edges.
   std::map<Vertex<T>, std::vector<Edge<T>>> adjacencies;

private:
   /// Helper function for numberOfPathsFrom().
   void pathsFrom(const Vertex<T> & source, const Vertex<T> & destination, std::set<Vertex<T>> & visited, int & pathCount) const;

   /// Helper function for topological sort.
   bool hasCycle(const Vertex<T> & source, std::set<Vertex<T>> & pushed) const;

   /// Helper function for topological sort, doing DFS
   bool topologicalSortDFS(const Vertex<T> & from,
                           std::set<Vertex<T>> & tmpMarked,
                           std::set<Vertex<T>> & permMarked,
                           std::vector<Vertex<T>> & topoList) const;
};


template <typename T>
void Graph<T>::addUndirectedEdge(const Vertex<T> & source, const Vertex<T> & destination, double weight) {
   // Adding an undirectional edge, so add a directed edge to both directions.
   addDirectedEdge(source, destination, weight);
   addDirectedEdge(destination, source, weight);
}

template <typename T>
void Graph<T>::add(EdgeType edge, const Vertex<T> & source, const Vertex<T> & destination, double weight) {
   switch (edge) {
      case EDirected:
         addDirectedEdge(source, destination, weight);
         break;
      case EUndirected:
         addUndirectedEdge(source, destination, weight);
         break;
   }
}

/*
 Creates a vertex and puts it in the map with an empty vector of edges.
 Returns a copy of the vertex to the caller.
 */
template <typename T>
Vertex<T> Graph<T>::createVertex(const T & data) {
   Vertex<T> vertex = Vertex<T>(data);
   adjacencies.insert(std::pair<Vertex<T>, std::vector<Edge<T>>>(vertex, {}));
   return vertex;
}

/*
 Adds a directed edge in the graph between source and destination.
 */
template <typename T>
void Graph<T>::addDirectedEdge(const Vertex<T> & source, const Vertex<T> & destination, double weight) {
   Edge<T> edge = Edge<T>(source, destination, weight);
   adjacencies.at(source).push_back(edge);
}

/*
 Copies vertices from the other graph.
 */
template <typename T>
void Graph<T>::copyVerticesFrom(const Graph<T> & graph) {
   for (const auto & vertexAndEdges : graph.adjacencies) {
      adjacencies[vertexAndEdges.first] = {};
   }
}

/*
 Retrieve the edges of a vertex.
 Returns a const reference to the bector containing the edges of the vertex.
 */
template <typename T>
const std::vector<Edge<T>> & Graph<T>::edges(const Vertex<T> & fromSource) const {
   return adjacencies.at(fromSource);
}

/*
 For a source vertex, get the weight of the destination vertex if there is an edge from
 the source to the destination. If there is no edge, returns 0.0.
 */
template <typename T>
double Graph<T>::weight(const Vertex<T> & fromSource, const Vertex<T> & toDestination) const {
   // Get the edges for the source vertex
   const auto & edgesarray = edges(fromSource);
   // Find an edge that has the specified destination.
   auto found = std::find_if(edgesarray.begin(), edgesarray.end(), [&toDestination](const Edge<T> & element) {
      return element.destination == toDestination; // lambda function returns true if destination is found
   });
   // If destination is found, found iterator is something else than the end iterator
   // and points to the found Edge.
   if (found != edgesarray.end()) {
      return found->weight; // Return the weight of the found edge.
   }
   return 0.0;  // In case edge was not found, return 0.0.
}

/// Streams the adjacency list to an output stream.
template <typename T>
std::ostream & operator << (std::ostream & stream, const Graph<T> & adjacencyList) {
   std::cout << "Adjacency list for the network with weights:" << std::endl;
   for (auto const& [key, values] : adjacencyList.adjacencies) {
      stream << key << " ---> [ ";
      int counter = 0;
      for (auto const & value : values) {
         stream << value;
         counter++;
         if (counter < values.size()) {
            stream << ", ";
         }
      }
      stream << " ]" << std::endl;
   }
   return stream;
}

template <typename T>
std::vector<Vertex<T>> Graph<T>::breadthFirstSearchFrom(const Vertex<T> & from) const {
   std::queue<Vertex<T>> queue;     // vertices to visit next
   std::set<Vertex<T>> enqueued;    // vertices already found and marked to be visited
   std::vector<Vertex<T>> visited;  // All visited vertices

   // Start from the provided vertex.
   queue.push(from);          // Push to vertices to visit
   enqueued.insert(from);     // and mark that is either visited or marked to be visited.

   while (!queue.empty()) {                        // Are there vertices to visit?
      auto vertex = queue.front();                 // Take the next one
      queue.pop();
      visited.push_back(adjacencies.find(vertex)->first);                   // Add it to the visited.
      auto neighborEdges = edges(vertex);          // Find the edges of this vertex; where can we go from here.
      for (auto const & edge : neighborEdges) {
         if (enqueued.find(edge.destination) == enqueued.end()) { // And it is not already marked to be visited
            queue.push(edge.destination);                         // Queue it to be visited
            enqueued.insert(edge.destination);                    // Remember this is either visited or marked to be visited.
         }
      }
   }
   return visited;      // Here we have all the vertices in the order they were found, breadth first.
}

template <typename T>
std::vector<Vertex<T>> Graph<T>::depthFirstSearchFrom(const Vertex<T> & from) const {
   std::stack<Vertex<T>> stack;        // vertices to depth search.
   std::set<Vertex<T>> pushed;         // Not yet handled in depth search
   std::vector<Vertex<T>> visited;     // vertices visited in depth search

   stack.push(from);                   // Where to start from
   pushed.insert(from);                // This is now (being) handled
   visited.push_back(adjacencies.find(from)->first);            // And also visited

   while (!stack.empty()) {            // While still something to depth search
      const auto & neighbours = edges(stack.top());   // Where to go from this vertex?
      if (neighbours.size() == 0) {                   // Nowhere, so take next from stack.
         stack.pop();
         continue;
      } else {                                        // Vertex has edges to go
         bool continueOuter = false;
         for (const auto & edge : neighbours) {       // For each neighbour...
            if (pushed.find(edge.destination) == pushed.end()) {  // If it hasn't been handled yet
               stack.push(edge.destination);                      //   - put this to stack to search
               pushed.insert(edge.destination);                   //   - is now searched
               visited.push_back(adjacencies.find(edge.destination)->first);   //   - and visited
               continueOuter = true;                              // Continue outer while loop to go
               break;                                             // deeper to items in stack (see *)
            }
         }
         if (continueOuter) {                                     // *) Found vertices to go deeper
            continue;                                             // into, so take them from stack
         }                                                        // and search there instead of pop **)
      }
      stack.pop();                                                // **) Did get deeper to one path, so
   }                                                              // now can pop vertex and take one
   return visited;                                                // from higher up the search tree.
}

/**
 Copy all vertices to an array to be used elsewhere.
 */
template <typename T>
std::vector<Vertex<T>> Graph<T>::allVertices() const {
   std::vector<Vertex<T>> vertices;
   for (const auto & element : adjacencies) {
      vertices.push_back(element.first);
   }
   return vertices;
}

/*
 Check if the graph has disconnected areas.
 */
template <typename T>
bool Graph<T>::isDisconnected() const {
   // First, get all vertices
   const auto & vertices = allVertices();
   // If there are none, sure it is not a disconnected graph.
   if (vertices.size() == 0) {
      return false;
   }

   // Do a breadh first search to get all vertices.
   const auto & visited = breadthFirstSearchFrom(vertices.at(0));
   // If you can find one vertice from all vertices that was not visited,
   // then the graph has disconnected areas.
   for (auto const & vertex : vertices) {
      if (std::find(visited.begin(), visited.end(), vertex) == visited.end()) {
         return true; // One of all vertices was not visited, so graph is disconnected.
      }
   }
   return false;
}

/*
 Search the number of paths that exist from source to destination.
 */
template <typename T>
int Graph<T>::numberOfPathsFrom(const Vertex<T> & source, const Vertex<T> & destination) const {
   int numberOfPaths = 0;
   std::set<Vertex<T>> visited; // Not needed here but used in recursive calls of pathsFrom(source, dest, visited, count).
   pathsFrom(source, destination, visited, numberOfPaths);
   return numberOfPaths;
}

/*
 Recursively search for paths from source to destination.
 */
template <typename T>
void Graph<T>::pathsFrom(const Vertex<T> & source, const Vertex<T> & destination, std::set<Vertex<T>> & visited, int & pathCount) const {
   // Mark this vertex as visited
   visited.insert(source);
   // If source is the destination, one more path found.
   if (source == destination) {
      pathCount += 1;
   } else {
      // Otherwise, for each edge, search for paths from the edge's destination
      // to the final destination, unless edge's destination has already been visited.
      auto neighbours = edges(source);
      for (const auto & edge : neighbours) {
         if (std::find(visited.begin(), visited.end(), edge.destination) == visited.end()) {
            pathsFrom(edge.destination, destination, visited, pathCount);
         }
      }
   }
   visited.erase(source);
}

/*
 Are there any cycles in the graph?
 This interface function calls the actual implementation (below) which finds
 the cycles.
 */
template <typename T>
bool Graph<T>::hasCycle(const Vertex<T> & source) const {
   // Using this set to handle the search...
   std::set<Vertex<T>> pushed;
   // ... calling this method, which calls itself recursively.
   return hasCycle(source, pushed);
}

/*
 Are there any cycles in the graph?
 This algorithm does a recursive depth first search to find cycles in the graph.
*/
template <typename T>
bool Graph<T>::hasCycle(const Vertex<T> & source, std::set<Vertex<T>> & pushed) const {
   pushed.insert(source);              // Put the current source vertex aside in the set.
   auto neighbours = edges(source);    // Get the edges from the source
   // For each edge...
   for (const Edge<T> & edge : neighbours) {
      // .. if the edge's destination is not in the set of handled vertices
      // ... and hasCycle returns true for edge's destination (in that path we have cycles),
      // then we have cycles!
      if (std::find(pushed.begin(), pushed.end(), edge.destination) == pushed.end()
          && hasCycle(edge.destination, pushed)) {
         return true;
      // ... or if the edge's destination has already been handled (is in the set), we have cycles!
      } else if (std::find(pushed.begin(), pushed.end(), edge.destination) != pushed.end()) {
         return true;
      }
   }
   pushed.erase(source);
   return false;     // Otherwise, we do not have cycles.
}

template <typename T>
std::vector<Vertex<T>> Graph<T>::topologicalSort() const {
   // This list will contain the nodes in topological sort order
   std::vector<Vertex<T>> topologicalList;
   // Nodes handling in process
   std::set<Vertex<T>> grayNodes;
   // Processed nodes
   std::set<Vertex<T>> blackNodes;

   // Go through all the nodes in the graph
   std::vector<Vertex<T>> allNodes = allVertices();
   while (!allNodes.empty()) {
      // While still nodes to handle, get one from the list
      Vertex<T> node = allNodes.front();
      allNodes.erase(allNodes.begin());
      // Does the DFS topological sort if not already handled for this node.
      if (blackNodes.find(node) == blackNodes.end()) {
         // If returns false, could not do this because cycles.
         if (!topologicalSortDFS(node, grayNodes, blackNodes, topologicalList)) {
            topologicalList.clear();
            break;
         }
      }
   }
   return topologicalList;
}

template <typename T>
bool Graph<T>::topologicalSortDFS(const Vertex<T> & from,
                                  std::set<Vertex<T>> & grayNodes,
                                  std::set<Vertex<T>> & blackNodes,
                                  std::vector<Vertex<T>> & topoList) const {
   // Terminates when it hits any node that has already been visited since the beginning of
   // the topological sort or the node has no outgoing edges (i.e. a leaf node).
   if (blackNodes.find(from) != blackNodes.end()) {
      return true;
   }
   // If node can be found from the "temporary" marked set, the graph
   // has cycles so return false. This will stop the search.
   if (grayNodes.find(from) != grayNodes.end()) {
      return false; // Stop searching, cycle in graph
   }
   // Now the node is in processing.
   grayNodes.insert(from);
   // Recursively handle outgoing edges from the node
   auto neighbours = edges(from);
   for (const Edge<T> & edge : neighbours) {
      if (!topologicalSortDFS(edge.destination, grayNodes, blackNodes, topoList)) {
         return false;
      }
   }
   // Node processed so remove it from grays and add to blacks.
   grayNodes.erase(from);
   blackNodes.insert(from);
   // And put the node in the beginning of the list.
   topoList.insert(topoList.begin(),from);
   return true;
}

#endif /* Graph_hpp */
