#include <iostream>

class triple{
  public:
    triple(int nterm,int ndoc,int freq);
    bool operator<(const triple& a) const;
    int nterm;
    int ndoc;
    int freq;
};
