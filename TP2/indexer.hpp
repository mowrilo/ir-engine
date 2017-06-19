#include <iostream>
#include "fileReader.hpp"
#include "parser.hpp"
#include <fstream>
// #include "triple.hpp"
#include <algorithm>
// #include "eliasCoding.hpp"
#include <bitset>
#include <cmath>
#include <memory>
#include <thread>
#include <mutex>
#include "nWayMergeSort.hpp"
#include "docList.hpp"

#define FILERUN "runs/run"
#define NTHREADS 4

using namespace std;
using namespace htmlcxx;

class indexer{
  public:
    indexer();
    static void index(string path_to_collection, int threadid, int* numberOfFiles); //método a ser executado em paralelo
    void start(string path_to_collection); //começa a indexação

  private:
    static ofstream docs;
    static vocabulary voc; //variáveis compartilhadas.
    static mutex docsFile;
    static mutex vocMutex;
    static mutex numberFile;
    static int fileToIndex;
    static int docNum;
    static documentList dlist;
};
