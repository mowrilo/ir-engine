#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "eliasCoding.hpp"

#define FILERUN "runs/run"

using namespace std;

class run{
  public:
    run(string name); //construtor, recebe o nome do arquivo
    vector<int> readAndDecode(); //lê os bytes e decodifica
    bool isEOF(); //checa fim de arquivo
    void close();
    void deleteFile(); //exclui o arquivo

  private:
    ifstream runFile;
    string fileName;
};
