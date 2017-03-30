#include <iostream>
using namespace std;

class url{
  public:
        url (string &nm);
        int getSize();
        //string noHttp(string &name);
        void setSize(string &name);
        string getName();
        // bool isBr();
        bool operator<(const url& a) const;

  private:
        string name;
        int size;
};
