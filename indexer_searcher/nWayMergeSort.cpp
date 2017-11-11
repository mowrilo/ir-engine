#include "nWayMergeSort.hpp"


int sortBlock::nBlocks;
queue<int, deque<int > > sortBlock::filesToMerge;
int sortBlock::numberOfFile;
mutex sortBlock::numFilesMut;
mutex sortBlock::queueMutex;
string sortBlock::path_to_runs;

sortBlock::sortBlock(int nBl,string pathToRuns){ //inicializa
    nBlocks = nBl;
    path_to_runs = pathToRuns;
}


void sortBlock::sortAll(int nFiles){
    cout << "Number of files to sort on folder " << path_to_runs << ": " << nFiles << ".\n";
    numberOfFile = nFiles;
    int topLevel = ceil(log2(nFiles)/log2(nBlocks));
    int value;
    int limInf = 0;
    int auxx = nFiles;
    int greaterFile = 0;
    while (nFiles != 1){ //cria uma fila de arquivos, e adiciona arquivos a cada nível de ordenação, até sobrar apenas 1
        for (int i=limInf; i<(limInf+auxx); i++){
            filesToMerge.push(i);
            if (i > greaterFile)    greaterFile = i;
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

vector<vector<int> > makeDiff(vector<vector<int> > docsToWrite){
    int cur = 0;
    for (int i=0; i<docsToWrite.size(); i++){
        int docAux = docsToWrite[i][1];
        docsToWrite[i][1] -= cur;
        cur = docAux;
    }
    return docsToWrite;
}

void sortBlock::writeAll(vector<vector<int> > docsToWrite, string file){
    int ndocs = docsToWrite.size();
    // cout << "ndocs: " << ndocs << "\n";
    int termId = docsToWrite[0][0];
    int nbytes = 0;
    // cout << "Oi\n";
    // docsToWrite = makeDiff(docsToWrite);
    // cout << "Oi2\n";
    stringstream ss;
    int fileNumber = termId/1000;
    ss << fileNumber;
    string fileName = file + ss.str();
    for (int i=0; i < docsToWrite.size(); i++){
        vector<int> v = docsToWrite[i];

        // cout << "olar " << v[0] << " " << v[1] << " " << v[2] << "\n";
        int n1;
        if (v[1] > 1){
            n1 = 2*floor(log2(v[1]))+1;
        }
        else{
            n1 = 2;
        }
        if (i==0){
            if (v[0] > 1){
                n1 += 2*floor(log2(v[0]))+1;
            }
            else{
                n1 += 2;
            }
        }
        int n2;// = 2*floor(log2(v[2]))+1;
        if (v[2] > 1){
            n2 = 2*floor(log2(v[2]))+1;
        }
        else{
            n2 = 2;
        }
        nbytes += ceil(((float) n1+n2)/7);
    }
    // cout << " nbytes: " << nbytes << "\n";
        // cout << "Term: " << termId << " on file " << fileName << "\n";
    eliasCoding::encodeAndWrite(termId,nbytes,docsToWrite.size(),fileName,true);
    // vector<int> previous(3);
    // for (vector<vector<int> >::iterator it=docsToWrite.begin(); it!=docsToWrite.end(); it++){
    // cout << "Size of docs to write: " << docsToWrite.size() <<"\n";
    // int prev = 0;
    for (int i=0; i<docsToWrite.size(); i++){
        // cout << "i: "    << i << "\n";
        vector<int> v = docsToWrite[i];
        bool ieq1 = (i == 0);
        // cout << "asd: " << (i < (docsToWrite.size() - 1)) << "\n";
        // if (i < (docsToWrite.size() - 1)){
        while((i < (docsToWrite.size() - 1)) && (docsToWrite[i+1][1] == v[1])){
                // cout << "iguais: " << v[1] << " " << docsToWrite[i+1][1] << "\n";
                v[2] += docsToWrite[i+1][2];
                i++;
        }

        // }
        // if (v[1] == previous[1]){
        //     previous[2] +=
        // }
        // int docWrite = v[1] - prev;
        eliasCoding::encodeAndWrite(v[0],v[1],v[2],fileName,ieq1);
        // prev = v[1];
        // for (int a=0; a<v.size();a++) cout << v[a] <<"\n";
    }
}

void sortBlock::buildIndex(int value){
    cout << "Building inverted index from run #" << value << "...\n";
    stringstream ss;
    ss << value;
    run last(path_to_runs + ss.str());
    int lastTerm=0, lastDoc = 0, docToWrite;
    // cout << "aegoooooo!\n";
    while(!last.isEOF()){
        vector<vector<int> > docsToWrite;
        vector<int> tuple = last.readAndDecode();
        docsToWrite.push_back(tuple);
        // cout << "eof: " << last.isEOF() <<"\n";
        lastTerm = tuple[0];
        // cout << "eof: " << last.isEOF() <<"\n";
        if (!last.isEOF()) tuple = last.readAndDecode();
        while ((tuple[0] == lastTerm) && (!last.isEOF())){
            // cout << tuple[0] << " " << tuple[1] << " " << tuple[2] << "\n";
            docsToWrite.push_back(tuple);
            tuple = last.readAndDecode();
        }
        lastTerm = tuple[0];

        ofstream arq;
        string paath = path_to_runs + "termN/";
        int fileNumber = (docsToWrite[0][0])/1000;
        paath.append(to_string(fileNumber));
        // cout << paath << "\n";
        arq.open(paath,ios::out | ios::app);
        arq << docsToWrite[0][0] << " " << docsToWrite.size() << "\n";
        arq.close();

        if (docsToWrite.size() > 0)   writeAll(docsToWrite, (path_to_runs + "invIndex"));
        // cout << "oi gente!\n";
        // if ((tuple[1] > 0) && (tuple[2] > 0)){ //lê as tuplas, e escreve no índice, podendo ou não adicionar o número do termo.
         //caso seja o mesmo termo, não adiciona.
        // lastTerm = tuple[0];
        // lastDoc = tuple[1];
        // }
    }
    cout << "Index built!\n";
    last.deleteFile();
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
            // shared_ptr<run> runAux(new run(path_to_runs + ss1.str()));
            run* runAux = new run(path_to_runs + ss1.str());
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
        string nameOfFile = path_to_runs + ss.str();
        cout << "Created " << nameOfFile << "\n";
        int prevTerm=0, prevDoc=0;

        for (int i=0; i<whichRuns.size(); i++){ //cria as primeiras triplas e adiciona na fila
            vector<int> eCod;
            if (!runVec[i]->isEOF()){
                eCod = runVec[i]->readAndDecode();
                // for (int asd = 0; asd<eCod.size(); asd++){
                //     cout << eCod[asd] << " ";
                // }
                // cout << "\n";
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
            //     docToWrite-=prevDoc;
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
                //     docToInsert += extracted.ndoc;
                // }
                triple insertTriple(eCod[0], eCod[1], eCod[2], nrun);
                pQueue.push(insertTriple);
            }
        }

        cout << "fechando arquivos:\n";
        for (int i=0; i<numm; i++){
            runVec[i]->close(); //fecha e remove os arquivos
            runVec[i]->deleteFile();
        }
        cout << "fechados!\n";
        // for (int i=0; i<runVec.size(); i++){
        //     delete[] runVec[i];
        // }
    }
    return;
}
