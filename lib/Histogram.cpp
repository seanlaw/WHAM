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
  convDim.clear();
  binwidth.clear();
  TOTAL=0;
}

void Histogram::updateMaxMin(const std::vector<double> &sin){
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
  this->updateMaxMin(sin);
  data.at(nfilein).push_back(sin);
}

void Histogram::setBins(const std::string &binsin){
  Misc::splitNum(binsin, ":", bins, false);
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

void Histogram::genHisto(const bool reduceFlag){
  unsigned int i;
  unsigned int j;
  unsigned int k;
  unsigned int b; //linearized histogram bin index

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

	Histo.resize(j);

  if (convDim.size() == 0){
    //Only set this once globally
    //May be a problem if new data gets read that has
    //different dimensions and/or bins
    convDim.resize(nDim);
    for (i=0; i< nDim; i++){
      convDim.at(i)=1;
      for (j=0; j< i; j++){
        convDim.at(i)=convDim.at(i)*bins.at(j);
      }
    }
  }

	for (b=0; b< Histo.size(); b++){
    Histo.at(b).setInx(b);
    this->getBinCoor(b);
    Histo.at(b).setLabel(this->getBinCoor(b));
  }	

  for (j=0; j< data.size(); j++){ //Each simulation window J
    for (k=0; k< data.at(j).size(); k++){ //Each datapoint K in J
      b=this->getBin(j, k);
			Histo.at(b).setN(Histo.at(b).getN()+1);
      TOTAL++;
      if (reduceFlag == true){
        //Reduce the dimensions to 1-D and store the bin
        data.at(j).at(k).resize(1);
        data.at(j).at(k).at(0)=b;
      }
    }
  }

}

unsigned int Histogram::getBin(const unsigned int &nfilein, const unsigned int &ndatain){
  unsigned int i;
  unsigned int j;
  unsigned int b;

  //nfilein = File index
  //ndatain = Datapoint line index in file nfilein

  b=0;

  if (binwidth.size() == 0){
    binwidth.resize(nDim);
    for (i=0; i< nDim; i++){
      if (i >= bins.size()){
        bins.push_back(defaultBins);
      }
      binwidth.at(i)=(MAX.at(i)-MIN.at(i))/bins.at(i);
    }
  }

  if (convDim.size() == 0){
    //Only set this once globally
    //May be a problem if new data gets read that has
    //different dimensions and/or bins
    convDim.resize(nDim);
    for (i=0; i< nDim; i++){
      convDim.at(i)=1;
      for (j=0; j< i; j++){
        convDim.at(i)=convDim.at(i)*bins.at(j);
      }
    }
  } 

  for (i=0; i< nDim; i++){ //Reaction coordinate I
    if (data.at(nfilein).at(ndatain).at(i) == MAX.at(i)){
      j=bins.at(i)-1;
    }
    else if (data.at(nfilein).at(ndatain).at(i) == MIN.at(i)){
      j=0;
    }
    else{
      j=static_cast<int>((data.at(nfilein).at(ndatain).at(i)-MIN.at(i))/binwidth.at(i));
    }
    //n-D to 1-D
    //Use B = Xbin + Y * (Xnbins) + Z*(Xnbins*Ynbins) + ...
    //Note that we are accumulating "b" as we pass through each dimension
    if (i==0){
      b=b+j;
    }
    else{
      b=b+j*convDim.at(i); //Build up one dimension at a time
    }
  }
  return b;
}

std::vector<unsigned int> Histogram::getBins (){
	return bins;
}

void Histogram::printHisto (HistoFormatEnum format, double temp){
  unsigned int i;
  unsigned int j;
  std::vector<unsigned int> convDim;
  std::vector<double> binwidth;
  double norm;
  std::vector<double> s;
  double kBT;
  double last;

  binwidth.resize(nDim);
  kBT=kB*temp;
  norm=1.0;
  last=std::numeric_limits<double>::min();

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
    norm*=binwidth.at(i);
  }

  convDim.resize(nDim);

  for (i=0; i< nDim; i++){
    convDim.at(i)=1;
    for (j=0; j< i; j++){
      convDim.at(i)=convDim.at(i)*bins.at(j);
    }
  }

	std::sort(Histo.begin(), Histo.end(), sortBinLabel);

  //1-D to n-D
	for (j=0; j< Histo.size(); j++){
		s=Histo.at(j).getLabel();
    if (last != s.at(0) && s.size() > 1){
      std::cout << std::endl;
      last=s.at(0);
    }
    for (i=0; i< s.size(); i++){
      std::cout << s.at(i) << "  ";
    }
    if (format == PROBABILITY){
			std::cout << static_cast<double>(Histo.at(j).getN())/TOTAL << std::endl;
    }
    else if (format == DENSITY){
      //Normalized probability, normalized by the bin width(s) (dx*dy*dz)
      //The area under the curve sums to one
      //The probability density is the height of the point (no width)
      //Use this when comparing shifts in the population (shifts in the area under the curve)
			std::cout << static_cast<double>(Histo.at(j).getN())/(TOTAL*norm) << std::endl;
    }
    else if (format == ENERGY){
      //Free energy from the probability
      //This is identical to the free energy calculated from the probability density
      //but shifted by a constant that is proportional to the bin width(s) (dx*dy*dz)
			std::cout << -kBT*log(static_cast<double>(Histo.at(j).getN())/TOTAL) << std::endl;
    }
    else{
      //Raw Histogram Count
			std::cout << Histo.at(j).getN() << std::endl;
    }
  }
}

std::vector<double> Histogram::getBinCoor(const unsigned int &bin){
  unsigned int rem;
  unsigned div;
  unsigned int i;
  std::vector<double> s;

  rem=bin;
  for (i=nDim-1; i != std::numeric_limits<unsigned int>::max(); i--){
   div=rem/convDim.at(i);
   rem=rem % convDim.at(i);
   s.push_back(MIN.at(i)+binwidth.at(i)*(static_cast<double>(div+0.5)));
  }

  std::reverse(s.begin(), s.end());

  return s;
}

unsigned int Histogram::getNFile(){
  return data.size();
}

unsigned int Histogram::getNData(int element){
  return data.at(element).size();
}

std::vector<Bin>& Histogram::getHisto(){
	return Histo;
}

unsigned int Histogram::getHistoSize(){
	return Histo.size();
}

bool Histogram::sortBinVal(const binpair &a, const binpair &b){
  for (unsigned int i=0; i< a.binval.size(); i++){
    if (a.binval.at(i) != b.binval.at(i)){
      return a.binval.at(i) < b.binval.at(i);
    }
  }
  
  return false;
}

bool Histogram::sortBinLabel(const Bin &a, const Bin &b){
	//Not entirely sure why I need to do this but it has to do with const
	Bin A, B;

	A=a;
	B=b;

  for (unsigned int i=0; i< A.getLabel().size(); i++){
    if (A.getLabel().at(i) != B.getLabel().at(i)){
      return A.getLabel().at(i) < B.getLabel().at(i);
    }
  }

  return false;
}
