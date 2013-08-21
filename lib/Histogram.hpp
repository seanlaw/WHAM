//Sean M. Law

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "Misc.hpp"

#include <vector>
#include <iostream>
#include <climits>

class Histogram {
  private:
    std::vector<unsigned int> HISTO; //Global Linear Histogram
    std::vector<double> MAX; //Global Max
    std::vector<double> MIN; //Global Min
    std::vector< std::vector< std::vector< double > > > data;
    std::vector<unsigned int> bins;
    unsigned int nDim;
    unsigned int defaultBins;

  public:
    Histogram(const unsigned &ninpin, const unsigned int &ndimin);
    void updateMAXMIN(const std::vector<double> &sin);
    void appendData(const std::vector<double> &sin, const unsigned int &nfilein);
    void setBins(const std::string &binsin);
    void setBins(const std::vector<unsigned int> &binsin);
    void setBins(const std::vector<int> &binsin);
    void genHISTOGRAM(const bool reduceFlag=false); //Global Histogram
    void printHISTOGRAM(); //Global Histogram
};

#endif
