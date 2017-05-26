#include "nWayMergeSort.hpp"

// class compare
// {
// public:
    // bool Compare(pair<triple,int> a, pair<triple,int> b){
    //   triple atr = a.first;
    //   triple btr = b.first;
    //   if (atr.nterm == btr.nterm) return atr.ndoc < btr.ndoc;
    //   return atr.nterm < btr.nterm;
    // }
// };


void nWayMergeSort::sort(int nfiles){
  vector<shared_ptr<run> > runVec;
  // run runVec[nfiles];
  // ifstream invIndex;

  priority_queue<triple, deque<triple> > pQueue;
  cout << "iniciando arquivos...\n";
  for (int i=0; i<nfiles; i++){
    // ifstream file;
    shared_ptr<run> runAux(new run(i));
    // stringstream ss;
    // ss << i;
    // runVec[i]
    // file.open(FILERUN + ss.str(), ios::in | ios::binary);
    runVec.push_back(runAux);
  }
  cout << "iniciou arquivos!\n";
  int prevTerm=0, prevDoc=0;

  cout << "primeiras triplas: \n";
  for (int i=nfiles-1; i>=0; i--){
    vector<int> eCod;
    if (!runVec[i]->isEOF()){
      eCod = runVec[i]->readAndDecode();
      cout << i << " < run " << eCod[0] << " < term " << eCod[1] << " < doc " << eCod[2] << " < freq\n";
      triple tr(eCod[0],eCod[1],eCod[2],i);
      // pair<triple, int> pr(tr, i);
      pQueue.push(tr);
    }
  }

  while (!pQueue.empty()){
    // break;
    // triple maior = pQueue.top();
    cout << "tamanho: " << pQueue.size();
    triple extracted = pQueue.top();
    cout << "tamanho: " << pQueue.size() << " extracted: " << extracted.nterm << " " << extracted.ndoc <<"\n";
    pQueue.pop();
    cout << "POP! tamanho: " << pQueue.size();
    break;
    int nrun = extracted.nrun;
    bool writeTerm = (extracted.nterm != prevTerm);
    //if (nrun == 0){
    cout << "extraiu: " << extracted.nterm << " " << extracted.ndoc << " " << extracted.freq << " vindo da run " << nrun << "\n";
    //}
    eliasCoding::encodeAndWrite(extracted.nterm, extracted.ndoc, extracted.ndoc, "invIndex", writeTerm);
    prevTerm = extracted.nterm;
    vector<int> eCod;
    if (!runVec[nrun]->isEOF()){
      eCod = runVec[nrun]->readAndDecode();
      int docToInsert = eCod[1];
      if (eCod[0] == prevTerm){
        docToInsert += extracted.ndoc;
      }
      cout << "received: " << eCod[0] << " " << eCod[1] << " " << eCod[2] << "\n";
      triple insertTriple(eCod[0], docToInsert, eCod[2], nrun);
      // extracted
      cout << "insere: " << insertTriple.nterm << " " << insertTriple.ndoc << " " << insertTriple.freq << "\n";
      pQueue.push(insertTriple);
    }
  }

  cout << "fechando arquivos:\n";
  for (int i=0; i<nfiles; i++){
    runVec[i]->close();
  }
  cout << "fechados!\n";
}
