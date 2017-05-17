#include <iostream>
#include <unordered_map>

using namespace std;

class vocabulary{
  public:
    vocabulary();
    void addTerm(string term);
    int getTermID(string term);

  private:
    mutex voc;
    unordered_map<string, int> vocab;
    int idMax;
};
