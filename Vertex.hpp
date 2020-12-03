//
//  Vertex.hpp
//  TrainTravelling
//
//  Created by Antti Juustila on 27.11.2020.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include <ostream>

template <typename T>
struct Vertex {
public:
   Vertex<T>() {
   };

   Vertex<T>(const T & data) {
      this->data = data;
   }

   Vertex<T>(const Vertex<T> & another) {
      this->data = another.data;
   }

   const Vertex<T> & operator = (const Vertex<T> & another) {
      if (this != &another) {
         this->data = another.data;
      }
      return *this;
   }

   T data;
};

template <typename T>
bool operator == (const Vertex<T> & lhs, const Vertex<T> & rhs) {
   return lhs.data == rhs.data;
}

template <typename T>
bool operator < (const Vertex<T> & lhs, const Vertex<T> & rhs) {
   return lhs.data < rhs.data;
}

template <typename T>
std::ostream & operator << (std::ostream & stream, const Vertex<T> & vertex) {
   stream << vertex.data << " ";
   return stream;
}

#endif /* Vertex_hpp */
