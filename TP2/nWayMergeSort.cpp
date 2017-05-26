#include "nWayMergeSort.hpp"

void nWayMergeSort::sort(int nfiles){
  vector<ifstream> files;
  ifstream invIndex;

  priority_queue<pair<triple, int>, deque<pair<triple, int> > > queue;
  for (int i=0; i<nfiles; i++){
    ifstream file;
    stringstream ss;
    ss << i;
    file.open(FILERUN + ss.str(), ios::in);
    files.push_back(file);
  }

  for (int i=0; i<nfiles; i++){
    vector<int> eCod;
    if (!files[i].eof()){
      eCod = eliasCoding::readAndDecode(files[i]);
      triple tr(eCod[0],eCod[1],eCod[2]);
      pair<triple, int> pr(tr, i);
      queue.push(pr);
    }
  }

  while (!queue.empty()){
    pair<triple, int> maior = queue.top();
    queue.pop();
    int nrun = maior.second;
    files[nrun]
  }
}
