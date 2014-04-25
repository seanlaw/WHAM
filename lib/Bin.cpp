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

#include "Bin.hpp"

Bin::Bin (){
  n=0;
  inx=0;
  label.clear();
}

void Bin::setN (const unsigned int &nin){
  n=nin;
}

void Bin::setInx (const unsigned int &inxin){
  inx=inxin;
}

void Bin::setLabel (const std::vector<double> &labelin){
  label=labelin;
}

unsigned int Bin::getN (){
  return n;
}

unsigned int Bin::getInx (){
  return inx;
}

std::vector<double> Bin::getLabel (){
  return label;
}

std::vector<double>& Bin::getLabelVec (){
  return label;
}

const std::vector<double>& Bin::getLabelVec () const{
  return label;
}
