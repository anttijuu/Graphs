//
//  Course.hpp
//  TrainTravelling
//
//  Created by Antti Juustila on 27.11.2020.
//

#ifndef Course_hpp
#define Course_hpp

#include <ostream>
#include <string>

struct Course {
public:
   Course()
   :code("Unknown"), abbr("abbr"), name(L"unknown"), year(0), period(0) {
   }

   Course(const std::string & aCode)
   : code(aCode), name(L"unknown"), abbr("abbr"), year(0), period(0) {
   }


   Course(const std::string & cod, const std::string & abb, const std::wstring & nam, int y, int p)
   : code(cod), abbr(abb), name(nam), year(y), period(p)  {
   }

   Course(const Course & another) {
      this->code = another.code;
      this->abbr = another.abbr;
      this->name = another.name;
      this->year = another.year;
      this->period = another.period;
   }

   const Course & operator = (const Course & another) {
      if (this != &another) {
         this->code = another.code;
         this->abbr = another.abbr;
         this->name = another.name;
         this->year = another.year;
         this->period = another.period;
      }
      return *this;
   }

   std::string code;
   std::string abbr; // abbreviated course name
   std::wstring name;
   int year;
   int period;
};

bool operator == (const Course & lhs, const Course & rhs) {
   return lhs.code == rhs.code;
}

bool operator < (const Course & lhs, const Course & rhs) {
   return lhs.code < rhs.code;
}

std::ostream & operator << (std::ostream & stream, const Course & vertex) {
   stream << vertex.abbr << " (" << vertex.code << ")";
   return stream;
}

#endif /* Course_hpp */
