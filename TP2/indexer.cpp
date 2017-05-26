#include "indexer.hpp"

#define NTHREADS 4

using namespace std;

ofstream indexer::docs;
int indexer::docNum;
mutex indexer::docsFile;
vocabulary indexer::voc;
mutex indexer::vocMutex;

indexer::indexer(){
}

void indexer::start(string path_to_collection){
  docs.open("docs", ios::out);
  docNum=1;
  vector<thread> thrds;
  for (int i=0; i<NTHREADS; i++){ //Inicia as threads sem semente
    thrds.push_back(thread(index, path_to_collection, i));
  }
  for (int i=0; i<NTHREADS; i++){
    thrds[i].join();
    cout << "joining thread " << i << "...\n";
  }
  docs.close();
  voc.print();
}


void indexer::index(string path_to_collection,int threadid){
  // encodeAndWrite(0,114,1,4);
  // encodeAndWrite(0,14,1,4);
  // readAndDecode(4);
  // readAndDecode(4);
  fileReader fr(path_to_collection, threadid, NTHREADS);
  parser ps;
  int runNum = threadid;
  while(fr.openNextFile()){
    vector<string> test = fr.getNextHtml();
    string url = test[0];
    string htmlCode = test[1];
    // docsFile.lock();
    // docs << docNum << " " << url << "\n";
    // docNum++;
    // docsFile.unlock();
    int tamanho = htmlCode.size();
    // ps.normalizeText(htmlCode);
    unordered_map<string,int> freqs;// = ps.parse(htmlCode);
    vector<triple> runVec;
    while (tamanho){
      docsFile.lock();
      docs << docNum << " " << url << "\n";
      docNum++;
      docsFile.unlock();
      ps.normalizeText(htmlCode);
      if (url.size() > 0) freqs = ps.parse(htmlCode);
      for (unordered_map<string,int>::iterator it=freqs.begin(); it != freqs.end(); it++){
        int termID;
        vocMutex.lock();
        voc.addTerm(it->first);
        termID = voc.getTermID(it->first);
        vocMutex.unlock();
        triple aux(termID, docNum, it->second, 0);
        runVec.push_back(aux);
      }
      test = fr.getNextHtml();
      url = test[0];
      htmlCode = test[1];
      tamanho = htmlCode.size();
    }
    vocMutex.lock();
    cout << "vocabulary size so far: " << voc.size() << "\n";
    vocMutex.unlock();
    cout << "beginning sorting...\n";
    sort(runVec.begin(),runVec.end());
    cout << "ended sorting!\n";
    cout << "first element: " << runVec[0].nterm << " " << runVec[0].ndoc << " " << runVec[0].freq << "\n";
    cout << "number of triples on run: " << runVec.size() << "\n";

    int diffTerm=0, diffDoc=0;
    int prevTerm=0, prevDoc=0;
    cout << "coding and writing...\n";
    // run runToWrite();
    for (vector<triple>::iterator it=runVec.begin(); it != runVec.end(); ++it){
      diffTerm = it->nterm - prevTerm;
      if (diffTerm == 0){
        diffDoc = it->ndoc - prevDoc;
      }
      else{
        diffDoc = it->ndoc;
      }
      // cout << it->nterm << " " << it->ndoc << " " << it->freq << "\n";
      stringstream ss;
      ss << runNum;
      string fileName = FILERUN + ss.str();
      eliasCoding::encodeAndWrite(it->nterm,diffDoc,it->freq,fileName,true);
      prevTerm = it->nterm;
      prevDoc = it->ndoc;
    }

    // cout << "maxDiff: " << maxDiff << "\n";
    // ps.printVoc();
    fr.closeFile();
    runNum+=NTHREADS;
  }
  // docs.close();
  // return 0;
}
