//Sean M. Law
//Aaron T. Frank
    
/*
This file is part of MoleTools.

MoleTools is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MoleTools is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with MoleTools.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "Enum.hpp"
#include "Bin.hpp"

#include <vector>
#include <string>

class Histogram {
  private:
    std::vector<Bin> Histo; //Global Linear Histogram
    std::vector<double> MAX; //Global Max
    std::vector<double> MIN; //Global Min
    std::vector< std::vector< std::vector< double > > > data;
    std::vector<unsigned int> bins;
    unsigned int nDim;
    unsigned int defaultBins;
    std::vector<unsigned int> convDim;
    std::vector<double> binwidth;
    unsigned int TOTAL;

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
    std::vector<Bin>& getHisto();
    unsigned int getHistoSize();
    static bool sortBinLabel(const Bin &a, const Bin &b);
};

#endif
