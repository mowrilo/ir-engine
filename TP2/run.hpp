#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "eliasCoding.hpp"

#define FILERUN "runs/run"

using namespace std;

class run{
  public:
    run(int nRun);
    vector<int> readAndDecode();
    bool isEOF();
    void close();

  private:
    ifstream runFile;
};
