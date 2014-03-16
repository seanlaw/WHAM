//Sean M. Law

#ifndef BIN_H
#define BIN_H

#include <vector>

class Bin {
  private:
    unsigned int n;
		unsigned int inx;
		std::vector<double> label;
	
	public:
		Bin();
		void setN(const unsigned int &nin);
		void setInx(const unsigned int &inxin);
		void setLabel(const std::vector<double> &labelin);
		unsigned int getN();
		unsigned int getInx();
		std::vector<double> getLabel();
		std::vector<double>& getLabelVec();
};

#endif

