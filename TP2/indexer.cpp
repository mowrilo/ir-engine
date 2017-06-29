#include "indexer.hpp"

using namespace std;

ofstream indexer::docs;
// int indexer::docNum;
mutex indexer::docsFile;
vocabulary indexer::voc("vocabulary/");
vocabulary indexer::anchor("anchorTerms/");
mutex indexer::vocMutex;
mutex indexer::anchorMutex;
mutex indexer::numberFile;
int indexer::fileToIndex;
documentList indexer::dlist;

indexer::indexer(){
    fileToIndex = 0;
}

bool isValid(string url){
    bool value = true;
    if (url.find(".pdf") != string::npos) value = false;
    if (url.find(".doc") != string::npos) value = false;
    if (url.find(".ppt") != string::npos) value = false;
    if (url.find(".mp3") != string::npos) value = false;
    return value;
}

vector<int> indexer::getDocTerms(int ndoc){
    string nameFile = "docs/docsTerms/docsterms";
    // cout << "ndoc: " << ndoc << "\n";
    int nfile = ndoc/1000;
    nameFile.append(to_string(nfile));
    nameFile += '_';
    for (int i=0; i<NTHREADS; i++){
        // ifstream arq;
        string nf = nameFile;
        nf.append(to_string(i));
        // arq.open(nf, ios::in);
        // cout << "filename: " << nf << "\n";
        run r(nf);
        if (r.isOpen()){
            vector<int> foo = r.readAndDecode();
            while ((foo[0] != ndoc) && (!r.isEOF())){
                // if (foo[0] != 0)    cout << "foo 0: " << foo[0] << "\n";
                r.seek(foo[1]);
                if (!r.isEOF()) foo = r.readAndDecode();
            }
            if (!r.isEOF()) foo = r.readAndDecode();
            if (foo[0] == ndoc){
                return foo;
            }
            r.close();
        }
    }
    vector<int> retorno;
    return retorno;
}

int indexer::getNDocs(int nterm){
    int nFile = nterm/1000;
    string path = "runs/runtermN/";
    path.append(to_string(nFile));
    ifstream file;
    file.open(path, ios::in);
    string entry;
    getline(file,entry);
    // cout << "entry: "<< entry << "\n";
    int pos=0;
    while (entry[pos] != ' ') pos++;
    int id = stoi(entry.substr(0,pos));
    int ndocs = stoi(entry.substr(pos+1,entry.size()));
    while ((id != nterm) && (!file.eof())){
        getline(file,entry);
        pos=0;
        while (entry[pos] != ' ') pos++;
        if (entry.size() > 0){
            id = stoi(entry.substr(0,pos));
            ndocs = stoi(entry.substr(pos+1,entry.size()));
        }
    }
    if (id == nterm)    return ndocs;
    return 0;
}

float indexer::getWd(int ndoc, int ndoctot){
    float ret = 0;
    // unordered_map<int,float> foo;
    // cout << "oi\n";
    vector<int> terms = getDocTerms(ndoc);
    if (terms.size() == 0)  return 0;
    // cout << "o2i\n";
    // cout << "oi size:" << terms.size() << "\n";
    for (int i=1; i<terms.size(); i+=2){
        int nterm = terms[i];
        float freq = (float) terms[i+1];
        // cout << "will get terms:\n";
        // vector<int> docums = getDocs(nterm);
        // cout << "got terms!\n";
        float docsnum = (float) getNDocs(nterm);
        float weight = 0;
        // cout << "parameters: " << freq << "    " << ndoctot << "    " << docsnum << "\n";
        if (docsnum != 0)   weight = (1 + log(freq))*(log(1+(((float) ndoctot)/docsnum)));
        // if (cons.find(nterm) != cons.end()){
        //     pair<int,int> aew(nterm,weight);
        //     foo.insert(aew);
        // }
        // cout << "weight: " << weight << "\n";
        ret += weight*weight;
    }
    // cout << "ndoc: " << ndoc << " score: " << sqrt(ret) << "\n";
    // pair<float, unordered_map<int, float> > retorna(sqrt(ret), foo);
    return sqrt(ret);
}

void indexer::calcWd(int nofd, int ii){
    string path = "docs/wds";
    path.append(to_string(ii));
    ofstream wds;
    wds.open(path, ios::out | ios::app);
    for (int i=1; i<=nofd; i++){
        if ((i % 4) == ii){
            float wd = getWd(i,nofd);
            if (wd > 0)   wds << i << " " << wd << "\n";
        }
    }
    wds.close();
}

void indexer::start(string path_to_collection){
    // docs.open("docs", ios::out);
    // docNum=1;
    vector<thread> thrds;
    int greater = 0;
    int nofd = 0;
    int nFiles[NTHREADS];
    int nDocs[NTHREADS];
    for (int i=0; i<NTHREADS; i++){ //Inicia as threads
        thrds.push_back(thread(index, path_to_collection, i, &nFiles[i], &nDocs[i]));
    }
    for (int i=0; i<NTHREADS; i++){
        thrds[i].join();
        cout << "joining thread " << i << "...\n";
    }
    for (int i=0; i<NTHREADS; i++){
        if (nFiles[i] > greater)    greater = nFiles[i]; //obtém o índice do maior arquivo
        if (nDocs[i] > nofd)    nofd = nDocs[i];
    }
    ofstream arq;
    arq.open("ndoc",ios::out);
    arq << nofd;
    arq.close();
    // docs.close();
    // voc.print();
    // anchor.print();
    // dlist.writeDomPR();
    // dlist.calcPagePR();
    dlist.calcDomPR();
    // cout << "RUN\n";
    sortBlock sbRun(10, "runs/run"); //ordena com 10 caminhos.
    // cout << "RUN\n";
    sbRun.sortAll(greater+1);
    // vector<thread> sortThreads;
    sortBlock sbAnc(10, "anchorRuns/run");
    sbAnc.sortAll(greater+1);
    vector<thread> thrds1;
    cout << "Calculating document weights...";
    for (int i=0; i<NTHREADS; i++){ //Inicia as threads
        thrds1.push_back(thread(calcWd, nofd, i));
    }
    for (int i=0; i<NTHREADS; i++){
        thrds1[i].join();
        cout << "joining thread " << i << "...\n";
    }
    // calcWd(nofd);
    // sortThreads.push_back(thread(sbRun.sortAll,greater));
    // sortThreads.push_back(thread(sbAnc.sortAll,greater));
}

bool compara(triple a, triple b){
    if (a.nterm == b.nterm){
        //if (a.ndoc)
        return (a.ndoc < b.ndoc);
    }
    return (a.nterm < b.nterm); //comparação para a ordenação
}

void indexer::index(string path_to_collection,int threadid, int *numberOfFiles, int *numberOfDocuments){
    *numberOfFiles = threadid;
    *numberOfDocuments = 0;
    fileReader fr(path_to_collection, threadid, NTHREADS); //inicializa o leitor de arquivos
    parser ps;
    int runNum = threadid;
    int nFile;
    numberFile.lock();
    nFile = fileToIndex;
    fileToIndex++;
    numberFile.unlock();
    // nFile = 920;
    while(fr.openNextFile(nFile)){ //abre um arquivo
        // cout << "thread " << threadid << " begun!!\n";
        *numberOfFiles = nFile;
        cout << "thread " << threadid << " Opened " << nFile << "\n";
        vector<string> test = fr.getNextHtml(); //obtém o próximo html
        // cout << "thread " << threadid << " TESTEEEEE " << nFile << "\n";
        string url = test[0];
        string htmlCode = test[1];
        int tamanho = htmlCode.size();
        vector<triple> runVec;
        vector<triple> runAnchor;
        while (tamanho > 3){ //enquanto houver um código válido
            string first4 = url.substr(0,4);
            if (first4.compare("http") != 0){
                url = "http://" + url
            }
            if ((url.size() > 10 && url.size() < 80) && isValid(url)){
                // break;
                // cout << "URL: " << url <<"\n";
                // cout << "html: " << htmlCode.size() << "\n";
                unordered_map<string,int> freqs;
                unordered_map<string,vector<string> > links;
                // cout << "lendo url: " << url << "\n";
                // if (url.compare("http://noticias.impa.br/auth?doc=2554") == 0)
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
                //     if (url.find("blogger") != string::npos){
                //             cout <<"URL: " << url << " NUMBER OF DOC: " << numberOfDoc << "\n";
                //     }
                    if (numberOfDoc > *numberOfDocuments)    *numberOfDocuments = numberOfDoc;
                    // cout << "adding doc " << numberOfDoc << " of length: " << freqs.size() << "\n";
                    // dlist.addLength(numberOfDoc, freqs.size());
                    for (unordered_map<string,vector<string> >::iterator it=links.begin(); it != links.end(); it++){
                        string urlPR = it->first;
                        // if ((urlPR.size() < 301) && (urlPR.size() > 10)){
                        //     string first4 = urlPR.substr(0,4);
                        //     if (first4.compare("http") == 0){
                                // cout << "url: " << urlPR << "\n";
                                PRVec.push_back(urlPR);
                                dlist.addUrl(urlPR);
                                docIDs.push_back(dlist.getDocId(urlPR));
                        //     }
                        // }
                        // cout << "saiu do \n";
                        // for (int i=0; i<it->second.size(); i++){
                        //     dlist.addAnchor(it->first, it->second[i]);
                        // }
                    }
                    // cout << "addedge começa\n";
                    // cout << url << "    " << PRVec.size() << "    " << docIDs.size() << "\n";
                    if (docIDs.size() > 0)    dlist.addEdge(url, PRVec, docIDs);
                    // cout << "addedge termina\n";
                    docsFile.unlock();
                    //     cout << "AH MEU AMIGO\n";
                    //     if (links.size() == 0)    cout << "MELHOR...\n";
                    // }
                    // if (links.size() > 0) {
                        int count = 0;
                        for (unordered_map<string,vector<string> >::iterator it=links.begin(); it != links.end(); it++){
                            // if (links.size() == 0) cout << "PUTA MERDA VEI\n";
                            vector<string> ancTerms = it->second;
                            // cout << "nfile: " << nFile << " links size: " << links.size() << " count: " << count << " size: " << docIDs.size() << "\n";
                            // for (unordered_map<string,vector<string> >::iterator itt=links.begin(); itt != links.end(); itt++)    cout << itt->first.size() <<"\n";
                            int docID = docIDs[count];
                            count++;
                            // cout << "nfile: " << nFile << " oioi1\n";
                            for (vector<string>::iterator iit=ancTerms.begin(); iit != ancTerms.end(); iit++){
                                if (ancTerms.size() == 0) cout << "ancSize: " << ancTerms.size() << "\n";
                                int termID;
                                anchorMutex.lock();
                                anchor.addTerm(*iit);
                                termID = anchor.getTermID(*iit);
                                anchorMutex.unlock();
                                triple aux(termID, docID, 1, 0);
                                // triple aux2(1, 1, 1, 0);
                                runAnchor.push_back(aux);
                            }
                            // cout << "nfile: " << nFile << " oioi2\n";
                        }
                    // }
                    // cout << "parsed!: " << url << "\n";
                    // ofstream termsDocs;
                    // stringstream ss;
                    // ss << numberOfDoc;
                    string docsFileName = "docs/docsTerms/docsterms";
                    int numOfFile = numberOfDoc/1000;
                    docsFileName.append(to_string(numOfFile));
                    docsFileName += '_';
                    docsFileName.append(to_string(threadid));
                    if (freqs.size() > 0)   eliasCoding::encodeAndWrite(numberOfDoc, freqs.size(), 1, docsFileName, true);
                    for (unordered_map<string,int>::iterator it=freqs.begin(); it != freqs.end(); it++){
                        int termID;
                        vocMutex.lock();
                        voc.addTerm(it->first); //obtém número dos termos e frequências,
                        termID = voc.getTermID(it->first); //criando triplas
                        vocMutex.unlock();
                        eliasCoding::encodeAndWrite(numberOfDoc, termID, it->second, docsFileName, (it == freqs.begin()));
                        triple aux(termID, numberOfDoc, it->second, 0);
                        runVec.push_back(aux);
                    }
                }
            }
            // cout << "saiu!\n";
            // cout << "got terms: " << url << "\n";
            test = fr.getNextHtml();
            url = test[0];
            htmlCode = test[1];
            tamanho = htmlCode.size();
        }
        cout << "thread " << threadid << " saiu!\n";
        vocMutex.lock();
        cout << "Vocabulary size so far: " << voc.size() << "\n";
        vocMutex.unlock();
        cout << "thread " << threadid << " Beginning sorting...\n";
        sort(runVec.begin(),runVec.end(), compara); //ordena a run
        sort(runAnchor.begin(),runAnchor.end(), compara); //ordena a run de anchors
        cout << "thread " << threadid << " Ended sorting!\n";
        cout << "Number of triples on run: " << runVec.size() << "\n";
        cout << "Number of triples on anchor run: " << runAnchor.size() << "\n";
        // int diffTerm=0, diffDoc=0;
        // int prevTerm=0, prevDoc=0;
        cout << "thread " << threadid << " Coding and writing...\n";
        for (vector<triple>::iterator it=runVec.begin(); it != runVec.end(); ++it){
            // if (it->nterm == prevTerm){
            //     diffDoc = it->ndoc - prevDoc;
            // }
            // else{
            //     diffDoc = it->ndoc;
            // }
            stringstream ss;
            ss << nFile;
            string fileName = "runs/run" + ss.str();
            eliasCoding::encodeAndWrite(it->nterm,it->ndoc,it->freq,fileName,true); //comprime e escreve em arquivo
            // prevTerm = it->nterm;
            // prevDoc = it->ndoc;
        }
        // cout << "thread " << threadid << " wrote runs\n";
        for (vector<triple>::iterator it=runAnchor.begin(); it != runAnchor.end(); ++it){
            // if (it->nterm == prevTerm){
            //     diffDoc = it->ndoc - prevDoc;
            // }
            // else{
            //     diffDoc = it->ndoc;
            // }
            stringstream ss;
            ss << nFile;
            string fileName = "anchorRuns/run" + ss.str();
            eliasCoding::encodeAndWrite(it->nterm,it->ndoc,it->freq,fileName,true); //comprime e escreve em arquivo
            // prevTerm = it->nterm;
            // prevDoc = it->ndoc;
        }
        // cout << "thread " << threadid << " wrote anchor\n";
        fr.closeFile();
        // cout << "thread " << threadid << " closed file\n";
        // nFile++;
        numberFile.lock();
        nFile = fileToIndex;
        fileToIndex++; //incrementa qual arquivo abrir em seguida
        numberFile.unlock();
        // if (fileToIndex >= 100)
        // break;
        // cout << "thread " << threadid << " got out\n";
    }
    return;
}
