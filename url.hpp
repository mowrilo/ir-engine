#include<iostream>
using namespace std;

class url{
  public:
        url (string &nm);
        int getSize();
        void setSize(string &name);

  private:
        string name;
        int size;
};
