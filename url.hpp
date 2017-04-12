#include <iostream>
using namespace std;

class url{
  public:
        url (string &nm, string &dm, int offset);
        int getWeight();
        int setWeight(string &name, int offset);
        string getName();
        string getDomain();
        void setValid(string &name);
        bool checkValid();
        void increaseWeight(int i);
        bool operator<(const url& a) const;

  private:
        string name;
        string domain;
        int weight;
        bool isValid;
};
