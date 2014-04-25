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

#include "WHAM.hpp"
#include "Misc.hpp"

#include <cstdlib>
#include <cmath>

void usage(){
  std::cerr << "Usage:   wham [-options] <metadatafile>" << std::endl;
  std::cerr << "Options: [-bins rcoor1[:rcoor2[:rcoor3]]]" << std::endl;
  std::cerr << "         [-iter value] [-tol value | -Ftol value]" << std::endl;
  std::cerr << "         [-temp T1[:T2...[[:TN[:Ttarget]]]] | -temp T1=TN=[incr[=Ttarget]]]" << std::endl;
  std::cerr << "         [-exp n:T1:T2]" << std::endl;
  std::cerr << "         [-fguess file | -fval file]" << std::endl;
  std::cerr << std::endl;
  exit(0);
}

int main (int argc, char **argv){

  int i;
  std::string currArg;
  std::vector<unsigned int> bins;
  double tol;
  unsigned int maxIter;
  WHAM *wham;
  std::string fguess;
  std::string fval;
 
  tol=1E-5;
  maxIter=1E6;
  wham=new WHAM;
  fguess.clear();
  fval.clear();

  //Copy original command
  for (i=0; i<argc; i++){
    wham->appendCmd(std::string(" ")+argv[i]);
  }

  for (i=1; i<argc; i++){
    currArg=argv[i];
    if (currArg.compare("-h") == 0 || currArg.compare("-help") == 0){
      usage();
    }
    else if (currArg.compare("-bins") == 0){
      currArg=argv[++i];
      wham->setBins(currArg);
    }
    else if (currArg.compare("-iter") == 0 || currArg.compare("-maxiter") == 0){
      currArg=argv[++i];
      std::stringstream(currArg) >> maxIter;
      wham->setMaxIter(maxIter);
    }
    else if (currArg.compare("-tol") == 0 || currArg.compare("-ftol") == 0){
      currArg=argv[++i];
      std::stringstream(currArg) >> tol;
      wham->setTol(tol);
    }
    else if (currArg.compare("-Ftol") == 0){
      currArg=argv[++i];
      std::stringstream(currArg) >> tol;
      tol=log(tol);
      wham->setTol(tol);
    }
    else if (currArg.compare("-temp") == 0 || currArg.compare("-temps") == 0){
      currArg=argv[++i];
      if (currArg.find(":") != std::string::npos || Misc::isdouble(currArg)){
        if (wham->setTemp(currArg) == true){
          usage();
        }
      }
      else if (currArg.find("=") != std::string::npos){
        if (wham->setTempRange(currArg) == true){
          usage();
        }
      }
      else{
        std::cerr << std::endl << "Error: Unrecognized temperature format";
        std::cerr << std::endl << std::endl;
        usage();
      }
    }
    else if (currArg.compare("-fval") == 0){
      currArg=argv[++i];
      fval=currArg;
      fguess.clear();
    }
    else if (currArg.compare("-fguess") == 0){
      currArg=argv[++i];
      fguess=currArg;
      fval.clear();
    }
    else if (currArg.compare("-exp") == 0){
      currArg=argv[++i];
      wham->getExpTempRange(currArg);
      return 0;
    }
    else if (currArg.compare(0,1,"-") == 0){
      std::cerr << "Warning: Skipping unknown option \"" << currArg << "\"" << std::endl;
    }
    else{
      wham->setMeta(currArg);
    }
  }

  if (wham->getMeta().length() <= 0){
    std::cerr << std::endl << "Error:  Please provide an input metadata file";
    std::cerr << std::endl << std::endl;
    usage();
  }

  if (wham->readMetadata() > 0){ //Check number of windows/files
    wham->processEnergies();
    if (fguess.size() != 0){
      wham->setFguess(fguess);
    }
    if (fval.size() != 0){
      wham->setFval(fval); //No WHAM iteration needed
    }
    else{
      wham->iterateWHAM();
      std::cout << "#" << wham->getCmd() << std::endl;
    }

    wham->setDenomInv();

    if (wham->processCoor() == true){ //Reaction coordinates
      wham->binOnTheFly();
      wham->printPMF();
    }
    else{
      std::cerr << "Error: The number of datapoints did not match up" << std::endl;
    }
  }
  
  return 0;
}
