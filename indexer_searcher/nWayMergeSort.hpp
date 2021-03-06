#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "triple.hpp"
#include "eliasCoding.hpp"
#include "run.hpp"
#include <memory>
#include <deque>
#include <cmath>
#include <utility>
#include <mutex>
#include <vector>
#include <thread>
#include <algorithm>
#include <memory>

// #define FILERUN "runs/run"
#define NTHREADSMERGE 4

using namespace std;

class sortBlock{
public:
  sortBlock(int nBl, string pathToRuns);
  void sortAll(int nFiles); //inicia a ordenação
  static void sort(int topLevel, int* value); //ordena em um nível
  void buildIndex(int value); //dado um arquivo ordenado com as runs, constrói o índice
  void writeAll(vector<vector<int> > docsToWrite, string file);

private:
  static int nBlocks;
  static int numberOfFile;
  static queue<int, deque<int> > filesToMerge;
  static mutex numFilesMut;
  static mutex queueMutex;
  static string path_to_runs;
};
