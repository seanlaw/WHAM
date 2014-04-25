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
    const std::vector<double>& getLabelVec() const;
};

#endif

