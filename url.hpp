#include <iostream>
using namespace std;

class url{
  public:
        url (string &nm, string &dm);
        int getWeight();
        //string noHttp(string &name);
        void setWeight(string &name);
        string getName();
        string getDomain();
        void setValid(string &name);
        bool checkValid();
        void increaseWeight();
        // bool isBr();
        bool operator<(const url& a) const;

  private:
        string name;
        string domain;
        int weight;
        bool isValid;
};
