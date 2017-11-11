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
#define FILEANCHOR "runsAnchor/run"
#define NTHREADS 4

using namespace std;
using namespace htmlcxx;

class indexer{
  public:
    indexer();
    static void index(string path_to_collection, int threadid, int* numberOfFiles, int *numberOfDocuments); //método a ser executado em paralelo
    void start(string path_to_collection); //começa a indexação
    static vector<int> getDocTerms(int ndoc);
    static int getNDocs(int nterm);
    static float getWd(int ndoc, int ndoctot);
    static void calcWd(int nofd, int ii);
    // bool isValid(string url);

  private:
    static ofstream docs;
    static vocabulary voc; //variáveis compartilhadas.
    static vocabulary anchor;
    static mutex docsFile;
    static mutex vocMutex;
    static mutex numberFile;
    static mutex anchorMutex;
    static int fileToIndex;
    // static int docNum;
    static documentList dlist;
};
