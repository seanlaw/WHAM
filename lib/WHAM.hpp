// Sean M. Law

#ifndef WHAM_H
#define WHAM_H

#include "Misc.hpp"
#include "Constants.hpp"
#include "Histogram.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

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
    std::vector< std::vector<double> > pdSum; //Partial Derivative for accelerated WHAM
    std::map<unsigned int, double> Pun; //Unbiased probabilities
    Histogram *rCoor; //Reaction coordinates

  public:
    WHAM ();
    void appendCmd(const std::string &str);
    void genWHAMInput();
    void readMetadata();
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
