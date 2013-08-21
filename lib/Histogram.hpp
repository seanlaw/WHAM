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
    std::vector<unsigned int> convDim;
    std::vector<double> binwidth;

  public:
    Histogram(const unsigned &ninpin, const unsigned int &ndimin);
    void updateMAXMIN(const std::vector<double> &sin);
    void appendData(const std::vector<double> &sin, const unsigned int &nfilein);
    void setBins(const std::string &binsin);
    void setBins(const std::vector<unsigned int> &binsin);
    void setBins(const std::vector<int> &binsin);
    void genHISTO(const bool reduceFlag=false); //Global Histogram
    unsigned int getBin(const unsigned int &nfilein, const unsigned int &ndatain); //For binning on the fly
    void printHISTO(); //Global Histogram
    std::vector<double> getBinCoor(const unsigned int &bin);
    unsigned int getNFile();
    unsigned int getNData(int element); 
};

#endif
