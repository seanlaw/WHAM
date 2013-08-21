//Sean M. Law

#include "Histogram.hpp"

Histogram::Histogram(const unsigned &ninpin, const unsigned int &ndimin){
  MIN.resize(ndimin);
  MAX.resize(ndimin);
  for (unsigned int i=0; i< ndimin; i++){
    MIN.at(i)=1E20;
    MAX.at(i)=-1E20;
  }
  data.resize(ninpin);
  nDim=ndimin;
  bins.clear();
  defaultBins=25;
}

void Histogram::updateMAXMIN(const std::vector<double> &sin){
  //Global Max/Min
  for (unsigned int i=0; i< MIN.size(); i++){
    if (sin.at(i) < MIN.at(i)){
      MIN.at(i)=sin.at(i);
    }
    if (sin.at(i) > MAX.at(i)){
      MAX.at(i)=sin.at(i);
    }
  }
}

void Histogram::appendData(const std::vector<double> &sin, const unsigned int &nfilein){
  this->updateMAXMIN(sin);
  data.at(nfilein).push_back(sin);
}

void Histogram::setBins(const std::string &binsin){
  Misc::splitNum(binsin, ":", bins);
}

void Histogram::setBins(const std::vector<unsigned int> &binsin){
  bins=binsin;
}

void Histogram::setBins(const std::vector<int> &binsin){
  unsigned int i;
  bins.resize(binsin.size());
  for (i=0; i< binsin.size(); i++){
    bins.at(i)= static_cast<unsigned int>(binsin.at(i));
  }
}

void Histogram::genHISTOGRAM(const bool reduceFlag){
  unsigned int i;
  unsigned int j;
  unsigned int k;
  unsigned int l;
  std::vector<unsigned int> convDim;
  unsigned int b; //linearized histogram bin index
  std::vector<double> binwidth;

  binwidth.resize(nDim);

  j=1; //Track total number of bins
  for (i=0; i< nDim; i++){
    if (i < bins.size()){
      j*=bins.at(i);
    }
    else{
      j*=defaultBins;
      bins.push_back(defaultBins);
    }
    binwidth.at(i)=(MAX.at(i)-MIN.at(i))/bins.at(i);
  }

  HISTO.resize(j,0);
  convDim.resize(nDim);

  for (i=0; i< nDim; i++){
    convDim.at(i)=1;
    for (j=0; j< i; j++){
      convDim.at(i)=convDim.at(i)*bins.at(j);
    }
  }

  for (j=0; j< data.size(); j++){ //Each simulation window J
    for (k=0; k< data.at(j).size(); k++){ //Each datapoint K in J
      b=0;
      for (i=0; i< nDim; i++){ //Reaction coordinate I
        if (data.at(j).at(k).at(i) == MAX.at(i)){
          l=bins.at(i)-1;
        }
        else if (data.at(j).at(k).at(i) == MIN.at(i)){
          l=0;
        }
        else{
          l=static_cast<int>((data.at(j).at(k).at(i)-MIN.at(i))/binwidth.at(i));
        }
        //n-D to 1-D
        //Use B = Xbin + Y * (Xnbins) + Z*(Xnbins*Ynbins) + ...
        //Note that we are accumulating "b" as we pass through each dimension
        if (i==0){
          b=b+l;
        }
        else{
          b=b+l*convDim.at(i); //Build up one dimension at a time
        }
      }
      HISTO.at(b)=HISTO.at(b)+1;
      if (reduceFlag == true){
        //Reduce the dimensions to 1-D and store the bin
        data.at(j).at(k).resize(1);
        data.at(j).at(k).at(0)=b;
      }
    }
  }
}

void Histogram::printHISTOGRAM (){
  unsigned int i;
  unsigned int j;
  unsigned int b;
  unsigned int rem;
  unsigned int div;
  std::vector<unsigned int> convDim;
  std::vector<double> binwidth;

  binwidth.resize(nDim);

  j=1; //Track total number of bins
  for (i=0; i< nDim; i++){
    if (i < bins.size()){
      j*=bins.at(i);
    }
    else{
      j*=defaultBins;
      bins.push_back(defaultBins);
    }
    binwidth.at(i)=(MAX.at(i)-MIN.at(i))/bins.at(i);
  }

  HISTO.resize(j,0);
  convDim.resize(nDim);

  for (i=0; i< nDim; i++){
    convDim.at(i)=1;
    for (j=0; j< i; j++){
      convDim.at(i)=convDim.at(i)*bins.at(j);
    }
  }


  //1-D to n-D
  for(b=0; b< HISTO.size(); b++){
    rem=b;
    for (i=nDim-1; i>=0 && i != UINT_MAX; i--){
      div=rem/convDim.at(i);
      rem=rem % convDim.at(i);
      std::cerr << MIN.at(i)+binwidth.at(i)*(static_cast<double>(div+0.5)) << " ";
    }
    std::cerr << HISTO.at(b) << std::endl;
  }
 
}
