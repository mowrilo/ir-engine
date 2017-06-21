#include "indexer.hpp"

using namespace std;

ofstream indexer::docs;
// int indexer::docNum;
mutex indexer::docsFile;
vocabulary indexer::voc("vocabulary");
vocabulary indexer::anchor("anchorTerms");
mutex indexer::vocMutex;
mutex indexer::anchorMutex;
mutex indexer::numberFile;
int indexer::fileToIndex;
documentList indexer::dlist;

indexer::indexer(){
  fileToIndex = 0;
}

void indexer::start(string path_to_collection){
  // docs.open("docs", ios::out);
  // docNum=1;
  vector<thread> thrds;
  int greater = 0;
  int nFiles[NTHREADS];
  for (int i=0; i<NTHREADS; i++){ //Inicia as threads
    thrds.push_back(thread(index, path_to_collection, i, &nFiles[i]));
  }
  for (int i=0; i<NTHREADS; i++){
    thrds[i].join();
    cout << "joining thread " << i << "...\n";
  }
  for (int i=0; i<NTHREADS; i++){
    if (nFiles[i] > greater)  greater = nFiles[i]; //obtém o índice do maior arquivo
  }

  // docs.close();
  voc.print();
  anchor.print();
  dlist.writeDomPR();
  sortBlock sbRun(10, "runs/run"); //ordena com 10 caminhos.
  sortBlock sbAnc(10, "anchorRuns/run");
  sbRun.sortAll(greater);
  sbAnc.sortAll(greater);
}

bool compara(triple a, triple b){
  return !(a < b); //comparação para a ordenação
}

void indexer::index(string path_to_collection,int threadid, int *numberOfFiles){
  *numberOfFiles = threadid;
  fileReader fr(path_to_collection, threadid, NTHREADS); //inicializa o leitor de arquivos
  parser ps;
  int runNum = threadid;
  int nFile;
  numberFile.lock();
  nFile = fileToIndex;
  fileToIndex++;
  numberFile.unlock();
  while(fr.openNextFile(nFile)){ //abre um arquivo
    *numberOfFiles = nFile;
    cout << "Opened " << nFile << "\n";
    vector<string> test = fr.getNextHtml(); //obtém o próximo html
    string url = test[0];
    string htmlCode = test[1];
    int tamanho = htmlCode.size();
    unordered_map<string,int> freqs;
    unordered_map<string,vector<string> > links;
    vector<triple> runVec;
    vector<triple> runAnchor;
    while (tamanho > 3){ //enquanto houver um código válido
      // cout << "lendo url: " << url << "\n";
      // if (url.compare("http://noticias.impa.br/auth?doc=2554") == 0)
      // cout << "html: " << htmlCode << "\n";
      if (htmlCode.find("%PDF") == string::npos){
        int numberOfDoc;
        // cout << "normalizing: " << url << "\n";
        ps.normalizeText(htmlCode);
        // cout << "parsing: " << url << "\n";
        info infoRet;
        if (url.size() > 0){
          infoRet = ps.parse(htmlCode); //parsing no código
          freqs = infoRet.termFreq;
          links = infoRet.linkTerm;
        }
        vector<string> PRVec;
        vector<int> docIDs;
        docsFile.lock();
        // numberOfDoc = docNum;
        // docNum++;
        dlist.addUrl(url);
        numberOfDoc = dlist.getDocId(url);
        dlist.addLength(numberOfDoc, freqs.size());
        for (unordered_map<string,vector<string> >::iterator it=links.begin(); it != links.end(); it++){
          string urlPR = it->first;
          if ((urlPR.size() < 301) && (urlPR.size() > 10)){
            string first4 = urlPR.substr(0,4);
            if (first4.compare("http") == 0){
              cout << "url: " << urlPR << "\n";
              PRVec.push_back(urlPR);
              dlist.addUrl(urlPR);
              docIDs.push_back(dlist.getDocId(urlPR));
            }
          }
          // for (int i=0; i<it->second.size(); i++){
          //   dlist.addAnchor(it->first, it->second[i]);
          // }
        }
        dlist.addEdge(url, PRVec, docIDs);
        docsFile.unlock();
        int count = 0;
        cout << "nfile: " << nFile << " oioi1\n";
        for (unordered_map<string,vector<string> >::iterator it=links.begin(); it != links.end(); it++){
          vector<string> ancTerms = it->second;
          int docID = docIDs[count];
          count++;
          for (vector<string>::iterator iit=ancTerms.begin(); iit != ancTerms.end(); iit++){
            int termID;
            anchorMutex.lock();
            anchor.addTerm(*iit);
            termID = anchor.getTermID(*iit);
            anchorMutex.unlock();
            triple aux(termID, docID, 1, 0);
            runAnchor.push_back(aux);
          }
        }
        cout << "nfile: " << nFile << " oioi2\n";
        // cout << "parsed!: " << url << "\n";
        for (unordered_map<string,int>::iterator it=freqs.begin(); it != freqs.end(); it++){
          int termID;
          vocMutex.lock();
          voc.addTerm(it->first); //obtém número dos termos e frequências,
          termID = voc.getTermID(it->first); //criando triplas
          vocMutex.unlock();
          triple aux(termID, numberOfDoc, it->second, 0);
          runVec.push_back(aux);
        }
      }
      // cout << "saiu!\n";
      // cout << "got terms: " << url << "\n";
      test = fr.getNextHtml();
      url = test[0];
      htmlCode = test[1];
      tamanho = htmlCode.size();
    }
    cout << "saiu!\n";
    vocMutex.lock();
    cout << "Vocabulary size so far: " << voc.size() << "\n";
    vocMutex.unlock();
    cout << "Beginning sorting...\n";
    sort(runVec.begin(),runVec.end(), compara); //ordena a run
    sort(runAnchor.begin(),runAnchor.end(), compara); //ordena a run de anchors
    cout << "Ended sorting!\n";
    cout << "Number of triples on run: " << runVec.size() << "\n";
    cout << "Number of triples on anchor run: " << runAnchor.size() << "\n";
    // int diffTerm=0, diffDoc=0;
    // int prevTerm=0, prevDoc=0;
    cout << "Coding and writing...\n";
    for (vector<triple>::iterator it=runVec.begin(); it != runVec.end(); ++it){
      // if (it->nterm == prevTerm){
      //   diffDoc = it->ndoc - prevDoc;
      // }
      // else{
      //   diffDoc = it->ndoc;
      // }
      stringstream ss;
      ss << nFile;
      string fileName = "runs/run" + ss.str();
      eliasCoding::encodeAndWrite(it->nterm,it->ndoc,it->freq,fileName,true); //comprime e escreve em arquivo
      // prevTerm = it->nterm;
      // prevDoc = it->ndoc;
    }
    for (vector<triple>::iterator it=runAnchor.begin(); it != runAnchor.end(); ++it){
      // if (it->nterm == prevTerm){
      //   diffDoc = it->ndoc - prevDoc;
      // }
      // else{
      //   diffDoc = it->ndoc;
      // }
      stringstream ss;
      ss << nFile;
      string fileName = "anchorRuns/run" + ss.str();
      eliasCoding::encodeAndWrite(it->nterm,it->ndoc,it->freq,fileName,true); //comprime e escreve em arquivo
      // prevTerm = it->nterm;
      // prevDoc = it->ndoc;
    }
    fr.closeFile();
    numberFile.lock();
    nFile = fileToIndex;
    fileToIndex++; //incrementa qual arquivo abrir em seguida
    numberFile.unlock();
  }
  return;
}
