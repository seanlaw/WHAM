//Sean M. Law

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "Enum.hpp"
#include "Misc.hpp"
#include "Constants.hpp"

#include <vector>
#include <iostream>
#include <limits>
#include <algorithm>

//Left outside of class to be accessed by WHAM
struct binpair {
  unsigned int bininx;               
  std::vector<double> binval;
};

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
    unsigned int TOTAL;
//    struct binpair {
//      unsigned int bininx;              
//      std::vector<double> binval;
//    };


  public:
    Histogram(const unsigned &ninpin, const unsigned int &ndimin);
    void updateMaxMin(const std::vector<double> &sin);
    void appendData(const std::vector<double> &sin, const unsigned int &nfilein);
    void setBins(const std::string &binsin);
    void setBins(const std::vector<unsigned int> &binsin);
    void setBins(const std::vector<int> &binsin);
    void genHisto(const bool reduceFlag=false); //Global Histogram
    unsigned int getBin(const unsigned int &nfilein, const unsigned int &ndatain); //For binning on the fly
		std::vector<unsigned int> getBins ();
    void printHisto(HistoFormatEnum format=COUNT, double temp=300); //Global Histogram
    std::vector<double> getBinCoor(const unsigned int &bin);
    unsigned int getNFile();
    unsigned int getNData(int element);
    std::vector<unsigned int>& getHisto();
    unsigned int getHistoSize();
    static bool sortBinVal(const binpair &a, const binpair &b);
};

#endif
