// Sean M. Law

#include "WHAM.hpp"

WHAM::WHAM (){
  cmd.clear();
  Fguess.clear();
  F.clear();
  expBVE.clear();
	expBVxEx.clear();
  nWindow=0;
  fMeta.clear();
  bins.clear();
  tol=1E-5;
  maxIter=1E6;
  B.clear();
  defaultT=300;
  B.push_back(1.0/(kB*defaultT));
  B0=1.0/(kB*1E-6);
  factor=1.0;
  factorFlag=false;
  denomInv.clear();
  Pun.clear();
}

void WHAM::appendCmd(const std::string &str){
  cmd+=str;
}

void WHAM::genWHAMInput(){

}

unsigned int WHAM::readMetadata(){
  std::string line;
  std::vector<std::string> s;
  std::ifstream metaFile;
  std::istream *metainp;


  line.clear();
  s.clear();
  metainp=NULL;

  metaFile.open(fMeta.c_str(), std::ios::in);
  metainp=&metaFile;
  while (metainp->good() && !(metainp->eof())){
    getline(*metainp, line);
    Misc::splitStr(line, " \t", s, false);
    if (s.size() == 3 && (s.at(0).compare("!") != 0 || s.at(0).compare("#") != 0)){
      inps.resize(this->getNWindow()+1);
      inps.at(this->getNWindow())=s;
      this->setNWindow(this->getNWindow()+1);
    }
  }

  if (metaFile.is_open()){
    metaFile.close();
  }

  if (this->getNWindow() > 0){
    this->fixTemp(); //Ensure the number of windows and temperatures match, assign B0 
  }

  return this->getNWindow();
}

void WHAM::processEnergies(){
  unsigned int i; //Simulation environment
  unsigned int j; //Simulation window
  unsigned int k; //Datapoint in simulation window j
  std::string vline; //Biasing potential
  std::string eline; //Total potential energy
  std::vector<double> v;
  std::vector<double> e;
  std::ifstream fvin;
  std::ifstream fein;
  std::istream *fvinp;
  std::istream *feinp;
  unsigned int lastVSize;

  fvinp=NULL;
  feinp=NULL;
  lastVSize=0;
  i=0;
  k=0; //Datapoint in simulation window j

  expBVE.resize(inps.size());

  for(j=0; j< inps.size(); j++){
    fvin.open(inps.at(j).at(0).c_str(), std::ios::in);
    fein.open(inps.at(j).at(1).c_str(), std::ios::in);
    fvinp=&fvin;
    feinp=&fein;

    if (fvinp->good() && feinp->good()){
      std::cerr << "Processing files \"" << inps.at(j).at(0) << "\" and \"" << inps.at(j).at(1) << "\"..." << std::endl;
      k=0;
      //Read both files
      while (fvinp->good() && !(fvinp->eof()) && feinp->good() && !(feinp->eof())){
        getline(*fvinp, vline);
        getline(*feinp, eline);
        Misc::splitNum(vline, " \t", v, false);
        Misc::splitNum(eline, " \t", e, false);
        if (vline.length() ==0){
          getline(*fvinp, vline);
          if (fvinp->eof()){
            continue;
          }
          else{
            std::cerr << "Error: File \"" << inps.at(j).at(0) << "\" contains too many lines" << std::endl;
            break;
          }
        }
        if (eline.length() == 0){
          getline(*feinp, eline);
          if (feinp->eof()){
            continue;
          }
          else{
            std::cerr << "Error: File \"" << inps.at(j).at(1) << "\" contains too many lines" << std::endl;
            break;
          }
        }
        if (j==0 && k == 0){
          lastVSize=v.size();
        }
        if (e.size() == 1 && this->getNWindow() == this->getTempSize()){
          if (v.size() == this->getNWindow() && v.size() == lastVSize){
            //Traditional WHAM
            expBVE.at(j).resize(k+1);
            expBVE.at(j).at(k).resize(this->getNWindow());
            for (i=0; i< this->getNWindow(); i++){
              expBVE.at(j).at(k).at(i)=exp(-B.at(i)*v.at(i)) * exp(-(B.at(i)-B0)*e.at(0));
            }
          }
          else if (v.size() == 2*this->getNWindow() && v.size() == lastVSize){
            //WHAM Extrapolation
            if (expBVxEx.size() == 0){
              expBVxEx.resize(inps.size());
            }
            expBVE.at(j).resize(k+1);
            expBVxEx.at(j).resize(k+1);
            expBVE.at(j).at(k).resize(this->getNWindow());
            expBVxEx.at(j).at(k).resize(this->getNWindow());
            for (i=0; i< this->getNWindow(); i++){
							//V = Vbias
              expBVE.at(j).at(k).at(i)=exp(-B.at(i)*v.at(i)) * exp(-(B.at(i)-B0)*e.at(0));
							//Vx = Vbias + Vextrapolation
              expBVxEx.at(j).at(k).at(i)=exp(-B.at(i)*(v.at(i)+v.at(i+this->getNWindow()))) * exp(-(B.at(i)-B0)*e.at(0));
            }
          }
          else{
            std::cerr << "Warning: File \"" << inps.at(j).at(0) << "\" line "<< k+1;
            std::cerr << " contains the wrong number of columns" << std::endl;
            continue;
          }
        }
        else{
          std::cerr << "Warning: File \"" << inps.at(j).at(1) << "\" line "<< k+1;
          std::cerr << " contains the wrong number of columns" << std::endl;
          continue;
        }
        k++;
      }
    }
    else if (fvinp->good()){
      std::cerr << "Processing file \"" << inps.at(j).at(0) << "\"..." << std::endl;
      k=0;
      //Read biasing potential only
      while (fvinp->good() && !(fvinp->eof())){
        getline(*fvinp, vline);
        Misc::splitNum(vline, " \t", v, false);
        if (vline.length() == 0){
          if (fvinp->eof()){
            continue;
          }
          else{
            std::cerr << "Error: File \"" << inps.at(j).at(0) << "\" contains too many lines" << std::endl;
            break;
          }
        }
        if (j==0 && k == 0){
          lastVSize=v.size();
        }
        if (v.size() == this->getNWindow() && v.size() == lastVSize){
          //Traditional WHAM
          expBVE.at(j).resize(k+1);
          expBVE.at(j).at(k).resize(this->getNWindow());
          for (i=0; i< this->getNWindow(); i++){
            expBVE.at(j).at(k).at(i)=exp(-B.at(i)*v.at(i));
          }
        }
        else if (v.size() == 2*this->getNWindow() && v.size() == lastVSize){
          //WHAM Extrapolation
          if (expBVxEx.size() == 0){
            expBVxEx.resize(inps.size());
          }
          expBVE.at(j).resize(k+1);
          expBVxEx.at(j).resize(k+1);
          expBVE.at(j).at(k).resize(this->getNWindow());
          expBVxEx.at(j).at(k).resize(this->getNWindow());
          for (i=0; i< this->getNWindow(); i++){
						//V = Vbias
            expBVE.at(j).at(k).at(i)=exp(-B.at(i)*v.at(i));
						//Vx = Vbias + Vextrapolation
            expBVxEx.at(j).at(k).at(i)=exp(-B.at(i)*(v.at(i)+v.at(i+this->getNWindow())));
          }
        }
        else{
          std::cerr << "Warning: File \"" << inps.at(j).at(0) << "\" line "<< k+1;
          std::cerr << " contains the wrong number of columns" << std::endl;
          continue;
        }
        k++; 
      }
    }
    else if (feinp->good()){
      std::cerr << "Processing file \"" << inps.at(j).at(1) << "\"..." << std::endl;
      k=0;
      //Read total potential only
      while (feinp->good() && !(feinp->eof())){
        getline(*feinp, eline);
        Misc::splitNum(eline, " \t", e, false);
        if (eline.length() == 0){
          if (feinp->eof()){
            continue;
          }
          else{
            std::cerr << "Error: File \"" << inps.at(j).at(1) << "\" contains too many lines" << std::endl;
            break;
          }
        }
        if (e.size() == 1){
          //Traditional WHAM
          expBVE.at(j).resize(k+1);
          expBVE.at(j).at(k).resize(this->getNWindow());
          for (i=0; i< this->getNWindow(); i++){
            expBVE.at(j).at(k).at(i)=exp(-(B.at(i)-B0)*e.at(0));
          }
        }
        else{
          std::cerr << "Warning: File \"" << inps.at(j).at(1) << "\" line "<< k+1;
          std::cerr << " contains more than one column" << std::endl;
          continue;
        }
        k++;
      }
    }
    else{
      std::cerr << "Warning: Files \"" << inps.at(i).at(0) << "\" and \"";
      std::cerr << inps.at(i).at(1) << "\" could not be read" << std::endl;
    }

    if (fvin.is_open()){
      fvin.close();
    }
    if (fein.is_open()){
      fein.close();
    }
  }
  std::cerr << std::endl;
}


bool WHAM::processCoor (){
  unsigned int j;
  unsigned int k;
  std::string line;
  std::vector<double> s;
  std::ifstream coorFile;
  std::istream *coorinp;
  unsigned int nDim;

  line.clear();
  s.clear();
  coorinp=NULL;
  nDim=0;

  for (j=0; j< this->getNWindow(); j++){
    std::cerr << "Processing file \"" << inps.at(j).at(2) << "\"..." << std::endl;
    k=0;
    coorFile.open(inps.at(j).at(2).c_str(), std::ios::in);
    coorinp=&coorFile;
    while (coorinp->good() && !(coorinp->eof())){
      getline(*coorinp, line);
      Misc::splitNum(line, " \t", s, false);
      if (line.length() ==0){
        getline(*coorinp, line);
        if (coorinp->eof()){
          continue;
        }
        else{
          std::cerr << "Error: File \"" << inps.at(j).at(2) << "\" contains too many lines" << std::endl;
          break;
        }
      }
      if (j==0 && k==0){
        nDim=s.size();
        rCoor=new Histogram(this->getNWindow(), nDim);
      } 
      if (s.size() == nDim){
        if (nDim > 1){
          //std::cerr << "Warning: WHAM can only handle 1-D PMFs" << std::endl;
        }
        rCoor->appendData(s, j);
        k++;
      }
      else{
        std::cerr << s.size() << " " << nDim << std::endl;
      }
    } 
    if (expBVE.at(j).size() != k){
      std::cerr << "Error: The number of datapoints do not match!" << std::endl;
      std::cerr << expBVE.at(j).size() << " " << k << std::endl;
    }
    if (coorFile.is_open()){
      coorFile.close();
    }
  }

  rCoor->setBins(this->getBins());

  std::cerr << std::endl;

  //Compare data size from reaction coordinate and expBVE
  if (expBVE.size() != rCoor->getNFile()){
    return false;
  }
  else{
    for (j=0; j< this->getNWindow(); j++){
      if (expBVE.at(j).size() != rCoor->getNData(j)){
        return false;
      }
    }
  }

  return true;
}

bool WHAM::iterateWHAM (){
  unsigned int i,j,k,l;
  unsigned int niter;
  std::vector<double> nFlast; //n(i)*exp(Bf(i))
  std::vector<double> FnextInv; //exp(-Bf(i)) = 1.0/[exp(Bf(i))]
  bool convergedFlag;
  double fnext; //Temporary variable
  double flast; //Temporary variable
  double FnextInvZero; //Temporary variable
  double df; //fabs(f(i,next) - f(i,last))
	time_t start;
	double stop;
 
  // WHAM Formalism (Adapted from Michael Andrec)
  //
  // 1.0/F(i) = 
  //
  //                                             exp(-B(i)V(i,jk))*exp[-(B(i)-B(0))E(i,jk)]
  // Sum(j=1,..,S) Sum(k=1,...,N(j)) --------------------------------------------------------------------
  //                                  Sum(l=1,..,S) N(l)*F(l)*exp(-B(l)V(l,jk))*exp[-(B(l)-B(0))E(l,jk)]
  //  

  // Comments
  // expBVE.size() = Number of simulations/windows, S = nWindow
	// expBVE.at(i).size() = Number of datapoints, N(i) in the ith simulation/window
  // expBVE.at(i).at(j).at(k) = exp(-B(i)V(i,jk))*exp[-(B(i)-B(0))E(i,jk)]
 
  if (expBVE.size() != this->getNWindow() || (expBVxEx.size() > 0 && expBVxEx.size() != this->getNWindow())){
    std::cerr << std::endl;
    std::cerr << "Error: The number of simulation windows do not match!";
    std::cerr << std::endl << std::endl;
    return true;
  }

  F.resize(this->getNWindow());
  nFlast.resize(this->getNWindow()); //n(j)*F(j)
  FnextInv.resize(this->getNWindow());
  denomInv.resize(this->getNWindow());

  for (j=0; j< this->getNWindow(); j++){
    //Initialize F
    nFlast.at(j)=expBVE.at(j).size();
    if (Fguess.size() > 0 && Fguess.size() == this->getNWindow()){
      nFlast.at(j)*=Fguess.at(j);
    }
    
    denomInv.at(j).resize(expBVE.at(j).size());
  }

  //WHAM Iterations
	time(&start);
  for (niter=1; niter< maxIter; niter++){
		for (i=0; i< this->getNWindow(); i++){
			FnextInv.at(i)=0.0;
		}
		
    for (j=0; j< this->getNWindow(); j++){ //For each simulation J
      for (k=0; k< expBVE.at(j).size(); k++){ //Foreach datapoint K in simulation J
       	//Calculate (redundant) denominator once for each iteration
				denomInv.at(j).at(k)=0.0;
        for (l=0; l< this->getNWindow(); l++){ //Foreach simulation environment L
          //Calculate denom
          denomInv.at(j).at(k)+=nFlast.at(l)*expBVE.at(j).at(k).at(l);
        }
        denomInv.at(j).at(k)=1.0/denomInv.at(j).at(k);
				for (i=0; i< this->getNWindow(); i++){ //For each F value I (simulation environment)
					if (expBVxEx.size() == expBVE.size()){ //WHAM Extrapolation
						FnextInv.at(i)+=expBVxEx.at(j).at(k).at(i)*denomInv.at(j).at(k);
					} 
					else{ //Traditional WHAM
         		FnextInv.at(i)+=expBVE.at(j).at(k).at(i)*denomInv.at(j).at(k);
					}
				}
      }
   	}

    //Check tolerance (note that tolerance is in f but F is in exp(Bf))
    convergedFlag=true;
    FnextInvZero=FnextInv.at(0); //Need this as FnextInv(0) gets shifted first
    for (i=0; i< this->getNWindow(); i++){
      //Shift FnextInv(i) relative to FnextInv(0)
      FnextInv.at(i)=FnextInv.at(i)/(pow(FnextInvZero, B.at(i)/B.at(0)));
      fnext=log(FnextInv.at(i))/(-B.at(i)); //Negative Beta
      flast=log(nFlast.at(i)/expBVE.at(i).size())/B.at(i); //Positive Beta
      df=fabs(fnext-flast);
      if (df >= tol){
        convergedFlag=false;
      }
    }

    //Update F values for next iteration
    for (i=0; i< this->getNWindow(); i++){
      nFlast.at(i)=expBVE.at(i).size()*(1.0/FnextInv.at(i)); //n(i)*F(i)
    }

    if (convergedFlag == true){
			stop=difftime(time(0), start);
      std::cout << "# Iteration = " << niter << " , Time = " << stop << " seconds " << std::endl;
      for (i=0; i< this->getNWindow(); i++){
        //Final exp(B(i)*f(i))
        F.at(i)=nFlast.at(i)/expBVE.at(i).size();
        flast=log(nFlast.at(i)/expBVE.at(i).size())/B.at(i);
        std::cout << "# f( " << i+1 << " ) = " << flast << std::endl;
      }
      break;
    }
    if (niter % 10 == 0){
      std::cerr << "# Iteration = " << niter << std::endl;
      for (i=0; i< this->getNWindow(); i++){
        flast=log(nFlast.at(i)/expBVE.at(i).size())/B.at(i);
        std::cerr << "# f( " << i+1 << " ) = " << flast << std::endl;
      }
    }
    else{
      std::cerr << "# Iteration = " << niter << std::endl;
    }
  }

  return false;
}

void WHAM::fixTemp(){
  while (B.size() < this->getNWindow()){
    std::cerr << "Warning: Simulation window " << B.size()+1 << " set to default temperature (";
    std::cerr << defaultT << " K)" << std::endl;
    B.push_back(1.0/(kB*defaultT));
  }

  if (B.size() > this->getNWindow()){
    B0=B.back();
    B.pop_back();
//    std::cerr << B.at(this->getNWindow()-1) << " " << B0 << std::endl;
  }

  if (B0 == 1.0/(kB*1E-6)){
    std::cerr << "Warning: Target temperature has been set to its default ("<< defaultT << " K)" << std::endl;
    B0=1.0/(kB*defaultT);
  }
  
  if (B.size() > this->getNWindow()){
    std::cerr << "Warning: Extra temperature(s) provided were ignored" << std::endl;
    B.resize(this->getNWindow());
  }
 
}

void WHAM::setMeta(const std::string &metain){
  fMeta=metain;
}

void WHAM::setBins(const std::string &binsin){
  Misc::splitNum(binsin, ":", bins, false);
}

void WHAM::setBins(const std::vector<unsigned int> &binsin){
  bins=binsin;
}

void WHAM::setBins(const std::vector<int> &binsin){
  unsigned int i;
  bins.resize(binsin.size());
  for (i=0; i< binsin.size(); i++){
    bins.at(i)= static_cast<unsigned int>(binsin.at(i));
  }
}

void WHAM::setTol(const double &tolin){
  tol=tolin;
}

void WHAM::setMaxIter(const unsigned int &iterin){
  maxIter=iterin;
}

bool WHAM::setTemp(const std::string &tin){
  B.clear();
  Misc::splitNum(tin, ":", B, false);
  if (B.size() <= 0){
    std::cerr << std::endl << "Error: Unrecognized temperature format " << tin;
    std::cerr << std::endl << std::endl;
    return true;
  }
  else{
    for (unsigned int i=0; i< B.size(); i++){
      B.at(i)=1.0/(kB*B.at(i));
    }
    return false;
  }
}

void WHAM::setTemp(const std::vector<double> &tin){
  B.clear();
  B=tin;
  for (unsigned int i=0; i< B.size(); i++){
    B.at(i)=1.0/(kB*B.at(i));
  }
}

bool WHAM::setTempRange(const std::string &tin){
  std::vector<double> s;
  unsigned int i;

  B.clear();
  Misc::splitNum(tin, "=", s, false);

  if (s.size() >= 3){
    for (i=0; i<= static_cast<unsigned int>((s.at(1)-s.at(0))/s.at(2)); i++){
      B.push_back(s.at(0)+i*s.at(2));
    }
    if (s.size() >=4){
      B.push_back(s.at(3)); //Target temp
    }
    for (i=0; i< B.size(); i++){
      B.at(i)=1.0/(kB*B.at(i));
    }
    return false;
  }
  else if (s.size() == 2){
    for (i=0; i<= static_cast<unsigned int>((s.at(1)-s.at(0))); i++){
      B.push_back(s.at(0)+i);
    }
    for (i=0; i< B.size(); i++){
      B.at(i)=1.0/(kB*B.at(i));
    }
    return false;
  }
  else{
    std::cerr << std::endl << "Error: Unrecognized temperature format " << tin;
    std::cerr << std::endl << std::endl;
    return true;
  }
}

void WHAM::getExpTempRange(const std::string &tin){
	std::vector<double> s;
	unsigned int i;
	double currT;
	double f;

	Misc::splitNum(tin, ":", s, false);

	if (s.size() == 3){
		f=exp(log(s.at(2)/s.at(1))/(static_cast<unsigned int>(s.at(0))-1));
		currT=s.at(1);
		for (i=0; i< s.at(0); i++){
			std::cout << std::fixed;
			std::cout << std::setprecision(4) << currT;
			if (i<s.at(0)-1){
				std::cout << ":";
			}
			else{
				std::cout << std::endl;
			}
			currT=currT*f;
		}
	}
}

void WHAM::setFactor(const double &factorin){
  factorFlag=true;
  factor=factorin;
}

void WHAM::setNWindow(const unsigned int &nwin){
  nWindow=nwin;
}

void WHAM::setNWindow(const int &nwin){
  nWindow=static_cast<unsigned int>(nwin);
}

void WHAM::setFguess(const std::string &fin){
  //Convert f(i) to Fguess(i)
  std::string line;
  std::vector<std::string> s;
  std::ifstream guessFile;
  std::istream *guessinp;
  double f;
  unsigned int j;
  unsigned int nline;

  line.clear();
  s.clear();
  guessinp=NULL;
  j=0;
  nline=0;

  Fguess.clear();
  F.clear();

  guessFile.open(fin.c_str(), std::ios::in);
  guessinp=&guessFile;
  while (guessinp->good() && !(guessinp->eof()) && j < this->getNWindow()){
    getline(*guessinp, line);
    Misc::splitStr(line, " \t", s, false);
    if (line.length() == 0){
      getline(*guessinp, line);
      if (guessinp->eof()){
        continue;
      }
      else{
        std::cerr << "Error: Found blank line (" << nline+1 << ") in file \"" << fin << "\"" << std::endl;
        break;
      }
    }
    if (Misc::isdouble(s.back())){
      std::stringstream(s.back()) >> f;
      Fguess.push_back(exp(B.at(j)*f)); //Fguess(i)=exp(B*f(i))
      j++;
    }
    nline++;
  }
  if(guessFile.is_open()){
    guessFile.close();
  }
}

void WHAM::setFval(const std::string &fin){
  //Convert f(i) to Fguess(i)
  std::string line;
  std::vector<std::string> s;
  std::ifstream fFile;
  std::istream *finp;
  double f;
  unsigned int j;
  unsigned int nline;

  line.clear();
  s.clear();
  finp=NULL;
  j=0;
  nline=0;

  Fguess.clear();
  F.clear();

  fFile.open(fin.c_str(), std::ios::in);
  finp=&fFile;
  while (finp->good() && !(finp->eof()) && j < this->getNWindow()){
    getline(*finp, line);
    Misc::splitStr(line, " \t", s, false);
    if (line.length() == 0){
      getline(*finp, line);
      if (finp->eof()){
        continue;
      }
      else{
        std::cerr << "Error: Found blank line (" << nline+1 << ") in file \"" << fin << "\"" << std::endl;
        break;
      }
    }
    if (Misc::isdouble(s.back())){
      std::stringstream(s.back()) >> f;
      F.push_back(exp(B.at(j)*f)); //Final F(i)=exp(B*f(i))
      j++;
    }
    nline++;
  }
  while (F.size() < this->getNWindow()){
    F.push_back(1);
    std::cerr << "Warning: Missing f( "<< F.size() << " ) value was replaced with default (0)" << std::endl;
  }
  if(fFile.is_open()){
    fFile.close();
  }
}

void WHAM::setDenomInv(){
  unsigned int j;
  unsigned int k;
  unsigned int l;
  
	denomInv.clear();
  denomInv.resize(this->getNWindow());
 
  for (j=0; j< this->getNWindow(); j++){ //For each simulation J
    denomInv.at(j).resize(expBVE.at(j).size());
    for (k=0; k< expBVE.at(j).size(); k++){ //Foreach datapoint K in simulation J
			
      denomInv.at(j).at(k)=0.0;
      for (l=0; l< this->getNWindow(); l++){ //Foreach simulation environment L
        //Calculate denom
				denomInv.at(j).at(k)+=expBVE.at(j).size()*F.at(l)*expBVE.at(j).at(k).at(l);
      }
      denomInv.at(j).at(k)=1.0/denomInv.at(j).at(k);
    }
  }

}

std::string WHAM::getMeta(){
  return fMeta;
}

unsigned int WHAM::getTempSize(){
  return B.size();
}

double WHAM::getTemp(const int &element){
  return 1.0/(kB*B.at(element));
}

std::string WHAM::getCmd(){
  return cmd;
}

unsigned int WHAM::getNWindow(){
  return nWindow;
}

std::vector<unsigned int> WHAM::getBins(){
  return bins;
}

void WHAM::binOnTheFly(){
  unsigned int b;
  std::map<unsigned int, double>::iterator it;
  double norm;

  norm=0.0;
	Pun.clear();

	//From Souaille & Roux, 2001
	//
	//				                                        
	// Pun = 
	//		                                                            N(j)
	// SUM(j=1,....,S) SUM(k=1,...,N(j)----------------------------------------------------------------------- Pb(j)
	//                                    SUM(l=1,...,S) N(l)*F(l)*exp(-B(l)V(l,jk))*exp[-(B(l)-B(0))E(l,jk)]
	//
	// But N(j)*Pb(j) = N(j)*[counts/N(j)] = Biased Histogram Counts, so
	//
	// Pun =
	//                                                           Biased Histogram Counts
	// SUM(j=1,....,S) SUM(k=1,...,N(j)-----------------------------------------------------------------------
	//                                    SUM(l=1,...,S) N(l)*F(l)*exp(-B(l)V(l,jk))*exp[-(B(l)-B(0))E(l,jk)]
	//
	// Note that one needs to re-weight each biased histogram count(jk) by expBVx(j,jk) when doing WHAM extrapolation!
	// i.e. only the Vextrapolation term of Vx = Vbias + Vextrapolation is needed, not Vbias
	//

  for (unsigned int j=0; j< this->getNWindow(); j++){
    for (unsigned int k=0; k < expBVE.at(j).size(); k++){
      //Note that we are using an std::map for storing the sparse Punbiased
      b=rCoor->getBin(j, k);
      if (expBVxEx.size() != expBVE.size()){
        //Traditional WHAM
				//The numerator is simply the biased histogram count
        if (Pun.find(b) != Pun.end()){
					Pun[b]+=denomInv.at(j).at(k);
        }
        else{
					Pun[b]=denomInv.at(j).at(k);
        }
      }
      else{
        //WHAM Extrapolation
				//The numerator is the biased histogram count re-weighted by expBVx for that datapoint
				//i.e. only the Vextrpolation term of Vx = Vbias + Vextrapolation is needed, not Vbias
        if (Pun.find(b) != Pun.end()){
          Pun[b]+=(expBVxEx.at(j).at(k).at(j)/expBVE.at(j).at(k).at(j))*denomInv.at(j).at(k);
        }
        else{
          Pun[b]=(expBVxEx.at(j).at(k).at(j)/expBVE.at(j).at(k).at(j))*denomInv.at(j).at(k);
        }
      }
    }
  }

  for (it=Pun.begin(); it != Pun.end(); it++){
    norm+=it->second;
  }

  for (it=Pun.begin(); it != Pun.end(); it++){
    it->second=it->second/norm;
  }
}

void WHAM::printPMF(){
  //Note that Pun has map keys that are already in sorted order
  std::map<unsigned int, double>::iterator it;
  std::vector<double> coor;
  unsigned int i, j, b;
	unsigned int nbins;
  double T;
	double last;
	std::vector<unsigned int> bins;
	std::vector<double> s;
	
	last=std::numeric_limits<double>::min();
  T=1.0/(B0*kB);

	bins=rCoor->getBins();
	nbins=1;
	for (i=0; i< bins.size(); i++){
		nbins*=bins.at(i);
	}

	rCoor->getHisto().resize(nbins);
  for (b=0; b< nbins; b++){
		rCoor->getHisto().at(b).setInx(b);
		rCoor->getHisto().at(b).setLabel(rCoor->getBinCoor(b));
  }

	//Sort based on the label, not the index
  std::sort(rCoor->getHisto().begin(), rCoor->getHisto().end(), Histogram::sortBinLabel);

	for (i=0; i< nbins; i++){
		b=rCoor->getHisto().at(i).getInx();
		coor=rCoor->getHisto().at(i).getLabel();
		if ((it=Pun.find(b)) != Pun.end()){
			if (last != coor.at(0) && coor.size() > 1 ){
				std::cout << std::endl;
				last=coor.at(0);
			}
    	for (j=0; j< coor.size(); j++){
      	std::cout << coor.at(j) << "   ";
    	}
    	std::cout << -kB*T*log(it->second) << "   ";
    	std::cout << it->second << std::endl;
		}
		else{
			//Zero probability bins
    	if (last != coor.at(0) && coor.size() > 1){
      	std::cout << std::endl;
      	last=coor.at(0);
    	}
    	for (j=0; j< coor.size(); j++){
      	std::cout << coor.at(j) << "   ";
    	}
    	std::cout << "NaN   ";
    	std::cout << "0.0" << std::endl;
		}
	}
}
