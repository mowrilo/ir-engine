#include<iostream>
using namespace std;

class url{
  public:
        url (string &nm);
        int getSize();
        string noHttp(string &name);
        void setSize(string &name);
        // bool isBr();

  private:
        string name;
        int size;
};
