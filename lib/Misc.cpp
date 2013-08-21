//Sean M. Law

#include "Misc.hpp"

std::string Misc::single (const std::string &resnamein){
	if (resnamein == "T"){

	}
	
	return "";
}

void Misc::splitStr (const std::string &str, const std::string &delim, std::vector<std::string> &out, const bool repeat){
  size_t p0=0;
  size_t p1=std::string::npos;
  size_t plast=std::string::npos;
	out.clear();

  //"repeat" = true means that a blank string is added when there are
  //back-to-back delimiters. Otherwise, repeat=false ignores back-to-back delimiters.

  plast=str.find_last_not_of(delim);
  p1=str.find_first_of(delim,p0);

  while (p1 != std::string::npos){
    if (p1-p0 > 0){
      out.push_back(str.substr(p0,p1-p0));
    }
    else{
      if (repeat){
        out.push_back(str.substr(p0,p1-p0));
      }
    }
    p0=p1+1;
    p1=str.find_first_of(delim, p0);
  }
  //After last delimiter
  if (plast != std::string::npos && plast >= p0 ){
    out.push_back(str.substr(p0,p1-p0));
  }
  else{
    if (repeat){
      out.push_back(str.substr(p0,p1-p0));
    }
  }
  //std::cerr << out.size() << std::endl;
}

template <class SplitVec>
void Misc::splitNum (const std::string &str, const std::string &delim, SplitVec &out, const bool repeat){
	out.clear();
	out.reserve(500); //Still need resizing but reduces moving of data in memory
  size_t p0=0;
  size_t p1=std::string::npos;
  size_t plast=std::string::npos;
	int n=0;

  //"repeat" = true means that a blank string is added when there are
  //back-to-back delimiters. Otherwise, repeat=false ignores back-to-back delimiters.

  plast=str.find_last_not_of(delim);
  p1=str.find_first_of(delim,p0);

  while (p1 != std::string::npos){
    if (p1-p0 > 0){
			out.resize(n+1);
			std::stringstream(str.substr(p0,p1-p0)) >> out.at(n);
			n++;
    }
    else{
      if (repeat){
				out.resize(n+1);
				std::stringstream(str.substr(p0,p1-p0)) >> out.at(n);
				n++;
      }
    }
    p0=p1+1;
    p1=str.find_first_of(delim, p0);
  }
  //After last delimiter
  if (plast != std::string::npos && plast >= p0){
		out.resize(n+1);
		std::stringstream(str.substr(p0,p1-p0)) >> out.at(n);
		n++;
  }
  else{
    if (repeat){
			out.resize(n+1);
			std::stringstream(str.substr(p0,p1-p0)) >> out.at(n);
			n++;
    }
  }
  //std::cerr << out.size() << std::endl;
}

template void Misc::splitNum<std::vector<int> > (const std::string&, const std::string&, std::vector<int>&, const bool);

template void Misc::splitNum<std::vector<unsigned int> > (const std::string&, const std::string&, std::vector<unsigned int>&, const bool);

template void Misc::splitNum<std::vector<double> > (const std::string&, const std::string&, std::vector<double>&, const bool);

template void Misc::splitNum<std::vector<float> > (const std::string&, const std::string&, std::vector<float>&, const bool);

bool Misc::isdigit (const std::string &str){
  return str.find_first_not_of("0123456789") == std::string::npos;
}

bool Misc::isdouble (const std::string &str){
  unsigned int nDecimals=0; 
  unsigned int negPos=0;

  for (unsigned int i=0; i< str.size(); i++){
    switch (str[i]){
      case '-': negPos=i;
        continue;
      case '.': nDecimals++;
        continue;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        continue;
      default:
        return false;
    }
  }
  if (nDecimals > 1 || negPos != 0){
    return false;
  }
  return true;
}

bool Misc::isfloat (const std::string &str){
  return isdouble(str);
}

bool Misc::isalpha (const std::string &str){
  return str.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") == std::string::npos;
}

bool Misc::isrange (const std::string &str){

  if (str.find_first_not_of("0123456789-") != std::string::npos){
    return false;
  }
  if (str.find("-") == std::string::npos){
    return false;
  }
  if (str.find_first_not_of("-") != 0 || str.find_last_not_of("-") < str.find("-")){
    return false;
  }

  return true;
}

std::string Misc::trim (const std::string &str, const std::string t){
  std::string out=str;

  size_t pos = out.find_last_not_of(t);
  if (pos != std::string::npos){
    if (out.length() != pos+1){
      out.erase(pos+1);
    }
    pos=out.find_first_not_of(t);
    if (pos != 0){
      out.erase(0, pos);
    }
  }
  else{
    out="";
  }

  return out;
}

std::string Misc::processRange (const std::string &start, const std::string &end){
  std::stringstream ss;
  int i, istart, iend;
  
  std::stringstream(start) >> istart;
  std::stringstream(end) >> iend;

  ss << istart;
  istart++;

  for (i=istart; i<=iend; i++){
    ss << "+" << i;
  }

  return ss.str();
}

void Misc::toupper (std::string &str){
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

double Misc::hypot (const double &a, const double &b){
	if (a !=0){
		return a*sqrt(1+(b/a)*(b/a));
	}
	else if (b !=0){
		return b*sqrt(1+(a/b)*(a/b));
	}
	else{
		return 0.0;
	}
}

bool Misc::sortPairFirst(const std::pair<int, int> &a, const std::pair<int, int> &b){
  return (a.first < b.first);
}


bool Misc::sortPairSecond(const std::pair<int, int> &a, const std::pair<int, int> &b){
	return (a.second < b.second);
}

bool Misc::findUniqueFirst(const std::pair<int, int> &a, const std::pair<int, int> &b){
	return (a.first == b.first);
}

bool Misc::findUniqueSecond(const std::pair<int, int> &a, const std::pair<int, int> &b){
	return (a.second == b.second);
}



