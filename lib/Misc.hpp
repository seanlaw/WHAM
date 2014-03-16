//Sean M. Law

#ifndef MISC_H
#define MISC_H

#include "Histogram.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <typeinfo>

class Misc {
  private:

  public:
    static void splitStr (const std::string &str, const std::string &delim, std::vector<std::string> &out, const bool repeat=true);
    template <class SplitVec>
      static void splitNum (const std::string &str, const std::string &delim, std::vector<SplitVec> &out, const bool repeat=true);
    static bool isdigit (const std::string &str);
    static bool isdouble (const std::string &str);
    static bool isfloat (const std::string &str);
    static bool isalpha (const std::string &str);
    static bool isrange (const std::string &str);
    static std::string trim (const std::string &str, const std::string t=" ");
    static std::string processRange (const std::string &start, const std::string &end);
    static void toupper (std::string &str);
		static int atoi (std::string &str, const unsigned int offset=0);
		static double hypot (const double &a, const double &b);
		template <class First, class Second>
			static bool sortPairFirst(const std::pair<First, Second> &a, const std::pair<First, Second> &b);
		template <class First, class Second>
			static bool sortPairSecond(const std::pair<First, Second> &a, const std::pair<First, Second> &b);
    template <class First, class Second>
		  static bool findUniqueFirst(const std::pair<First, Second> &a, const std::pair<First, Second> &b);
    template <class First, class Second>
		  static bool findUniqueSecond(const std::pair<First, Second> &a, const std::pair<First, Second> &b);
};

#endif
