//
//  Station.hpp
//  TrainTravelling
//
//  Created by Antti Juustila on 27.11.2020.
//

#ifndef Station_hpp
#define Station_hpp

#include <ostream>
#include <string>

struct Station {
public:
   Station()
   :name("Unknown"), phone("unknown"), opens(0), closes(0) {
   }

   Station(const std::string & aName)
   : name(aName), phone("unknown"), opens(0), closes(0) {
   }


   Station(const std::string & aName, const std::string & aPhone, int open, int close)
   : name(aName), phone(aPhone), opens(open), closes(close)  {
   }

   Station(const Station & another) {
      this->name = another.name;
      this->phone = another.phone;
      this->opens = another.opens;
      this->closes = another.closes;
   }

   const Station & operator = (const Station & another) {
      if (this != &another) {
         this->name = another.name;
         this->phone = another.phone;
         this->opens = another.opens;
         this->closes = another.closes;
      }
      return *this;
   }

   std::string name;
   std::string phone;
   int opens;
   int closes;
};

bool operator == (const Station & lhs, const Station & rhs) {
   return lhs.name == rhs.name;
}

bool operator < (const Station & lhs, const Station & rhs) {
   return lhs.name < rhs.name;
}

std::ostream & operator << (std::ostream & stream, const Station & vertex) {
   stream << vertex.name << " ";
   return stream;
}

#endif /* Station_hpp */
