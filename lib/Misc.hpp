//Sean M. Law

#ifndef MISC_H
#define MISC_H

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
		static std::string single (const std::string &resnamein);
    static void splitStr (const std::string &str, const std::string &delim, std::vector<std::string> &out, const bool repeat=true);
		template <class SplitVec>
			static void splitNum (const std::string &str, const std::string &delim, SplitVec &out, const bool repeat=true);
    static bool isdigit (const std::string &str);
    static bool isdouble (const std::string &str);
    static bool isfloat (const std::string &str);
    static bool isalpha (const std::string &str);
    static bool isrange (const std::string &str);
    static std::string trim (const std::string &str, const std::string t=" ");
    static std::string processRange (const std::string &start, const std::string &end);
    static void toupper (std::string &str);
		static double hypot (const double &a, const double &b);
		static bool sortPairFirst(const std::pair<int, int> &a, const std::pair<int, int> &b);
		static bool sortPairSecond(const std::pair<int, int> &a, const std::pair<int, int> &b);
		static bool findUniqueFirst(const std::pair<int, int> &a, const std::pair<int, int> &b);
		static bool findUniqueSecond(const std::pair<int, int> &a, const std::pair<int, int> &b);
};

#endif
