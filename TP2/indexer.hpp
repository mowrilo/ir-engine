#include <iostream>
#include "fileReader.hpp"
#include "parser.hpp"
#include <fstream>
#include "triple.hpp"
#include <algorithm>
#include "eliasCoding.hpp"
#include <bitset>
#include <cmath>
#include <memory>
#include <thread>
#include <mutex>

//#define PATH_HTMLS "/media/murilo/hdext/collection/"//"/home/murilo/Documentos/ri/TP2/"//

using namespace std;
using namespace htmlcxx;

class indexer{
  public:
    indexer();
    static void index(string path_to_collection, int threadid);
    //static void encodeAndWrite(int a, int b, int c, int runNum);
    //void readAndDecode(int runNum);
    void start(string path_to_collection);

  private:
    static ofstream docs;
    static vocabulary voc;
    static mutex docsFile;
    static mutex vocMutex;
    static int docNum;
};
