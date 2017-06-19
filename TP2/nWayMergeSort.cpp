#include "nWayMergeSort.hpp"


int sortBlock::nBlocks;
queue<int, deque<int > > sortBlock::filesToMerge;
int sortBlock::numberOfFile;
mutex sortBlock::numFilesMut;
mutex sortBlock::queueMutex;

sortBlock::sortBlock(int nBl){ //inicializa
  nBlocks = nBl;
}


void sortBlock::sortAll(int nFiles){
  numberOfFile = nFiles;
  int topLevel = ceil(log2(nFiles)/log2(nBlocks));
  int value;
  int limInf = 0;
  int auxx = nFiles;
  int greaterFile = 0;
  while (nFiles != 1){ //cria uma fila de arquivos, e adiciona arquivos a cada nível de ordenação, até sobrar apenas 1
    for (int i=limInf; i<(limInf+auxx); i++){
      filesToMerge.push(i);
      if (i > greaterFile)  greaterFile = i;
      cout << "Pushed file number " << i << "\n";
    }
    limInf +=auxx;
    vector<thread> thrds;
    cout << "creating threads...\n";
    for (int i=0; i<NTHREADSMERGE; i++){ //Inicia as threads
      thrds.push_back(thread(sort, topLevel, &value));
    }
    for (int i=0; i<NTHREADSMERGE; i++){
      thrds[i].join();
      cout << "joining thread " << i << "...\n";
    }
    int aux = nFiles;
    nFiles = nFiles/nBlocks;
    if ((aux % nBlocks) != 0) nFiles++; //calcula quantos arquivos serão no próximo nível
    auxx = nFiles - filesToMerge.size();
  }
  value = greaterFile+1;
  buildIndex(value); //cria o índice
}

void writeAll(vector<vector<int> > docsToWrite, string file){
  int ndocs = docsToWrite.size();
  int termId = docsToWrite[0][0];
  int nbytes = 0;
  for (vector<vector<int> >::iterator it=docsToWrite.begin(); it!=docsToWrite.end(); it++){
    vector<int> v = *it;
    int n1 = 2*floor(log2(v[1]))+1;
    int n2 = 2*floor(log2(v[2]))+1;
    nbytes += ceil(((float) n1+n2)/7);
  }
  eliasCoding::encodeAndWrite(termId,nbytes,docsToWrite.size(),file,true);
  for (vector<vector<int> >::iterator it=docsToWrite.begin(); it!=docsToWrite.end(); it++){
    vector<int> v = *it;
    eliasCoding::encodeAndWrite(termId,v[1],v[2],file,false);
  }
}

void sortBlock::buildIndex(int value){
  cout << "Building inverted index from run #" << value << "...\n";
  stringstream ss;
  ss << value;
  run last(FILERUN + ss.str());
  int lastTerm=0, lastDoc = 0, docToWrite;
  vector<vector<int> > docsToWrite;
  vector<int> tuple = last.readAndDecode();
  while(!last.isEOF()){
    docsToWrite.push_back(tuple);
    lastTerm = tuple[0];
    tuple = last.readAndDecode();
    while ((tuple[0] == lastTerm) && (!last.isEOF())){
      docsToWrite.push_back(tuple);
      tuple = last.readAndDecode();
    }
    lastTerm = tuple[0];
    writeAll(docsToWrite, "indIndex");
    // if ((tuple[1] > 0) && (tuple[2] > 0)){ //lê as tuplas, e escreve no índice, podendo ou não adicionar o número do termo.
     //caso seja o mesmo termo, não adiciona.
    // lastTerm = tuple[0];
    // lastDoc = tuple[1];
    // }
  }
  cout << "Index built!\n";
}

void sortBlock::sort(int topLevel, int* value){
  while (true){
    priority_queue<triple, deque<triple> > pQueue; //fila de prioridades para as triplas
    vector<int> whichRuns;
    queueMutex.lock();
    if (filesToMerge.size() <= 1){ //caso tenha um ou nenhum arquivo, sai da função
      queueMutex.unlock();
      return;
    }
    // vector<shared_ptr<run> > runVec;
    vector<run*> runVec;
    int numm=0;
    while ((numm < nBlocks) && (!filesToMerge.empty())){ //adiciona arquivos para intercalar
      int which = filesToMerge.front();
      cout << "Got to sort: " << which << "\n";
      filesToMerge.pop();
      stringstream ss1;
      ss1 << which;
      // shared_ptr<run> runAux(new run(FILERUN + ss1.str()));
      run* runAux = new run(FILERUN + ss1.str());
      whichRuns.push_back(which);
      runVec.push_back(runAux);
      numm++;
    }
    vector<int> lastTerm(nBlocks);
    vector<int> lastDoc(nBlocks);
    queueMutex.unlock();
    stringstream ss;
    int thisFile;
    numFilesMut.lock();
    thisFile = numberOfFile;
    numberOfFile++;
    numFilesMut.unlock();
    ss << thisFile;
    cout << "Created " << thisFile << "\n";
    string nameOfFile = FILERUN + ss.str();
    int prevTerm=0, prevDoc=0;

    for (int i=0; i<whichRuns.size(); i++){ //cria as primeiras triplas e adiciona na fila
      vector<int> eCod;
      if (!runVec[i]->isEOF()){
        eCod = runVec[i]->readAndDecode();
        triple tr(eCod[0],eCod[1],eCod[2],i);
        pQueue.push(tr);
      }
    }
    while (!pQueue.empty()){
      triple extracted = pQueue.top(); //retira o topo
      pQueue.pop();
      int nrun = extracted.nrun;
      // bool writeTerm = (extracted.nterm != prevTerm);
      // int docToWrite = ;
      // if (extracted.nterm == prevTerm){ //caso o termo seja igual ao anterior, guarda apenas a diferença
      //   docToWrite-=prevDoc;
      // }
      // if ((docToWrite > 0) && (extracted.freq > 0)){
        eliasCoding::encodeAndWrite(extracted.nterm, extracted.ndoc, extracted.freq, nameOfFile, true);
        // prevTerm = extracted.nterm;
        // prevDoc = extracted.ndoc;
      // }
      vector<int> eCod;
      if (!runVec[nrun]->isEOF()){
        eCod = runVec[nrun]->readAndDecode();
        // int docToInsert = eCod[1]; //lê um novo termo na run que originou o extraído
        // if (eCod[0] == extracted.nterm){
        //   docToInsert += extracted.ndoc;
        // }
        triple insertTriple(eCod[0], eCod[1], eCod[2], nrun);
        pQueue.push(insertTriple);
      }
    }

    cout << "fechando arquivos:\n";
    for (int i=0; i<numm; i++){
      runVec[i]->close(); //fecha e remove os arquivos
      // runVec[i]->deleteFile();
    }
    cout << "fechados!\n";
    // for (int i=0; i<runVec.size(); i++){
    //   delete[] runVec[i];
    // }
  }
  return;
}