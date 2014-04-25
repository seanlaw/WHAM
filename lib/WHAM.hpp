// Sean M. Law
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

#ifndef WHAM_H
#define WHAM_H

#include <vector>
#include <map>
#include <string>

//Forward Declaration
class Histogram;

class WHAM {
  private:
    std::string cmd;
    std::vector<double> Fguess;
    std::vector<double> F; //Final exp(B(i)*f(i))
    std::vector< std::vector< std::vector<double> > > expBVE; //Is dynamic and can be jagged
    std::vector< std::vector< std::vector<double> > > expBVxEx; //Is dynamic and can be jagged
    unsigned int nWindow;
    std::string fMeta;
    std::vector<unsigned int> bins;
    double tol;
    unsigned int maxIter;
    std::vector<double> B; //Beta(i) = E.size() 
    double B0; //Target temp, BetaZero
    double defaultT;
    bool factorFlag;
    double factor; //For use with Molecular Transfer Model (MTM)
    std::vector< std::vector<std::string> > inps;
    std::vector< std::vector<double> > denomInv; //Inverse denominator for WHAM calculation
    std::map<unsigned int, double> Pun; //Unbiased probabilities
    Histogram *rCoor; //Reaction coordinates

  public:
    WHAM ();
    void appendCmd(const std::string &str);
    void genWHAMInput();
    unsigned int readMetadata();
    void processMetadata(const std::string &metatype);
    void processEnergies(); 
    bool iterateWHAM();
    bool processCoor(); //Reaction coord
    void fixTemp();

    void setMeta(const std::string &metain);
    void setBins(const std::string &binsin);
    void setBins(const std::vector<unsigned int> &binsin);
    void setBins(const std::vector<int> &binsin);
    void setTol(const double &tolin);
    void setMaxIter(const unsigned int &iterin);
    bool setTemp(const std::string &tin);
    void setTemp(const std::vector<double> &tin);
    bool setTempRange(const std::string &tin);
    void getExpTempRange(const std::string &tin);
    void setFactor(const double &factorin);
    void setNWindow(const unsigned int &nwin);
    void setNWindow(const int &nwin);
    void setFguess(const std::string &fin);
    void setFval(const std::string &fin);
    void setDenomInv();

    std::string getMeta();
    unsigned int getTempSize();
    double getTemp(const int &element);
    std::string getCmd();
    unsigned int getNWindow();
    std::vector<unsigned int> getBins();
    void binOnTheFly();
    void printPMF();
};

#endif
