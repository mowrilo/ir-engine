#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class fileReader{
  public:
    fileReader(string path, int initNum, int numThreads);
    vector<string> getNextHtml(); //obtém próximo html do arquivo
    bool openNextFile(int nFile); //abre próximo arquivo. retorna true se arquivo existir e false caso contrário
    bool closeFile(); //fecha arquivo

  private:
    ifstream file;
    string path_to_files;
    int fileNumber;
    int nthreads;
};
