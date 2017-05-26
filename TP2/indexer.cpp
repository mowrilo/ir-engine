#include "indexer.hpp"

#define NTHREADS 7

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
    docsFile.lock();
    docs << docNum << " " << url << "\n";
    docNum++;
    docsFile.unlock();
    int tamanho = htmlCode.size();
    ps.normalizeText(htmlCode);
    unordered_map<string,int> freqs = ps.parse(htmlCode);
    vector<triple> run;
    while (tamanho){
      test = fr.getNextHtml();
      url = test[0];
      htmlCode = test[1];
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
        triple aux(termID, docNum, it->second);
        run.push_back(aux);
      }
      tamanho = htmlCode.size();
    }
    cout << "beginning sorting...\n";
    sort(run.begin(),run.end());
    cout << "ended sorting!\n";
    cout << "ntriples: " << run.size() << "\n";

    int diffTerm=0, diffDoc=0;
    int prevTerm=0, prevDoc=0;
    cout << "coding and writing...\n";
    for (vector<triple>::iterator it=run.begin(); it != run.end(); ++it){
      diffTerm = it->nterm - prevTerm;
      if (diffTerm == 0){
        diffDoc = it->ndoc - prevDoc;
      }
      else{
        diffDoc = it->ndoc;
      }
      // cout << it->nterm << " " << it->ndoc << " " << it->freq << "\n";
      eliasCoding::encodeAndWrite(diffTerm,diffDoc,it->freq,runNum);
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
