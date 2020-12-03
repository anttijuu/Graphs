//
//  Edge.hpp
//  TrainTravelling
//
//  Created by Antti Juustila on 27.11.2020.
//

#ifndef Edge_hpp
#define Edge_hpp

#include "Vertex.hpp"

template <typename T>
struct Edge {
public:

   Edge<T>() {
      weight = 1.0;
   }

   Edge<T>(const Edge<T> & another) {
      this->source = another.source;
      this->destination = another.destination;
      this->weight = another.weight;
   }

   Edge<T>(const Vertex<T> & source, const Vertex<T> & destination, double weight) {
      this->source = source;
      this->destination = destination;
      this->weight = weight;
   }

   const Edge<T> & operator = (const Edge<T> & another) {
      if (this != &another) {
         this->source = another.source;
         this->destination = another.destination;
         this->weight = another.weight;
      }
      return *this;
   }

   Vertex<T> source;
   Vertex<T> destination;
   double weight;
};

template <typename T>
bool operator == (const Edge<T> & lhs, const Edge<T> & rhs) {
   return lhs.source == rhs.source;
}

template <typename T>
int operator < (const Edge<T> & lhs, const Edge<T> & rhs) {
   return lhs.source < rhs.source;
}

template <typename T>
std::ostream & operator << (std::ostream & stream, const Edge<T> & edge) {
   stream << edge.destination.data << " " << edge.weight;
   return stream;
}

#endif /* Edge_hpp */
