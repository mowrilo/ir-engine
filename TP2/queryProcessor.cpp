#include "queryProcessor.hpp"

queryProcessor::queryProcessor(void){
    ifstream a;
    a.open("ndoc");
    int ndoctot1;
    a >> ndoctot1;
    ndoctot = ndoctot1;
    cout << ndoctot << "\n";
    a.close();
}

int queryProcessor::getTermID(string termToFind){
    ifstream voc;
    string FileName = "vocabulary/";
    FileName += termToFind[0];
    // cout << FileName << "\n";
    voc.open(FileName, ios::in);
    string term, entry;
    getline(voc,entry); //lê cada entrada do vocabulário, com nome e id do termo
    int id;
    bool found = voc.eof();
    while (!found && !voc.eof()){
        int pos=0;
        while (entry[pos] != ' ')    pos++;
        term = entry.substr(0,pos); //lê linearmente o vocabulário, até encontrar o termo ou
        id = stoi(entry.substr(pos+1,entry.size()));                 // acabar o arquivo
        // cout << "term: " << term << "\n";
        found=(termToFind.compare(term) == 0);
        getline(voc,entry);
    }
    voc.close();
    if (!found){
        return 0;
    }
    return id;
}

int queryProcessor::getAnchorNum(string termToFind){
    ifstream voc;
    string FileName = "anchorTerms/";
    FileName += termToFind[0];
    // cout << FileName << "\n";
    voc.open(FileName, ios::in);
    string term, entry;
    getline(voc,entry); //lê cada entrada do vocabulário, com nome e id do termo
    int id;
    bool found = voc.eof();
    while (!found && !voc.eof()){
        int pos=0;
        while (entry[pos] != ' ')    pos++;
        term = entry.substr(0,pos); //lê linearmente o vocabulário, até encontrar o termo ou
        id = stoi(entry.substr(pos+1,entry.size()));                 // acabar o arquivo
        // cout << "term: " << term << "\n";
        found=(termToFind.compare(term) == 0);
        getline(voc,entry);
    }
    voc.close();
    if (!found){
        return 0;
    }
    return id;
}

vector<int> queryProcessor::getDocs(int termId){//string termToFind){
    // int termId = getTermID(termToFind); //encontra o ID do termo
    // cout << "entrou getdocs\n";
    if (termId == 0){
        vector<int> empty;
        return empty;
    }
    string FileName = "runs/runinvIndex";
    // cout << "termid: " << termId << "\n";
    int FileNum = termId/1000;
    FileName.append(to_string(FileNum));
    // cout << "set name\n";
    run r(FileName);
    // cout << "termid\n";// << termId;
    vector<int> ret = r.readAndDecode();
    while ((ret[0] != termId) && (!r.isEOF())){
        // cout << "term: \n";
        // if (ret.size() == 3)   for (vector<int>::iterator it = ret.begin(); it != ret.end(); ++it) cout << *it << " ";
        // cout <<"retsize: " << ret.size() << "\n";
        r.seek(ret[1]);
        ret = r.readAndDecode(); //lê o índice até encontrar o termo em questão
    }
    cout << "frequencia na coleção: " << ret[2] << " docs in a total of " << ndoctot << "\n";
    ret = r.readAndDecode();
    r.close();
    vector<int> retorno;
    int curDoc = 0;
    for (int i=1; i<ret.size(); i++){
        // if (i%2 == 1){
            curDoc = ret[i];
            // cout << "doc: " << curDoc << "\n";
            retorno.push_back(curDoc); //lê todos os documentos da lista
        // }
    }
    if (ret[0] != termId){
        vector<int> empty;
        return empty;
    }
    return retorno;
}

string queryProcessor::findDoc(int docId){
    // if (docId >= 300000)    return "";
    ifstream docs;
    string FileName = "docs/urls/";
    // cout << "searching... " << docId << "\n";
    int FileNum = docId/1000;
    FileName.append(to_string(FileNum));
    docs.open(FileName,ios::in);
    string term, entry;
    getline(docs,entry);
    int pos=0;
    while (entry[pos] != ' ') pos++;
    int id = stoi(entry.substr(0,pos));
    term = entry.substr(pos+1,entry.size());
    while ((id != docId) && (!docs.eof())){ //procura no arquivo de documentos, linearmente
        getline(docs,entry);
        pos=0;
        while (entry[pos] != ' ') pos++;
        if (entry.size() > 0){
            // cout << "entry: " << entry.size() << "\n";
            id = stoi(entry.substr(0,pos)); //separa o nome da ID
            // cout << "oi\n";
            term = entry.substr(pos+1,entry.size());
        }
    }
    if (docs.eof()){
        return "";
    }
    return term;
    docs.close();
}

vector<int> queryProcessor::mergeTerms(vector<int> term1, vector<int> term2){
    int pos1=0, pos2=0;
    vector<int> all;
    while ((pos1 < term1.size()) && (pos2 < term2.size())){ //mantém dois ponteiros,
        if (term1[pos1] < term2[pos2])    pos1++;                             //que é uma boa solução para listas ordenadas.
        else if (term1[pos1] > term2[pos2])    pos2++;
        else{
            all.push_back(term1[pos1]);
            pos1++;
            pos2++;
        }
    }
    return all;
}

vector<int> queryProcessor::getDocTerms(int ndoc){
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

int queryProcessor::getNDocs(int nterm){
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

pair<float, unordered_map<int, float> > queryProcessor::getWd(int ndoc, unordered_set<int> cons){
    float ret = 0;
    unordered_map<int,float> foo;
    // cout << "oi\n";
    vector<int> terms = getDocTerms(ndoc);
    // cout << "o2i\n";
    // cout << "oi size:" << terms.size() << "\n";
    for (int i=1; i<terms.size(); i+=2){
        int nterm = terms[i];
        float freq = (float) terms[i+1];
        // cout << "will get terms:\n";
        // vector<int> docums = getDocs(nterm);
        // cout << "got terms!\n";
        float docsnum = (float) getNDocs(nterm);
        // cout << "parameters: " << freq << "    " << ndoctot << "    " << docsnum << "\n";
        if (docsnum == 0) docsnum++;
        float weight = (1 + log(freq))*(log(1+(((float) ndoctot)/docsnum)));
        if (cons.find(nterm) != cons.end()){
            pair<int,int> aew(nterm,weight);
            foo.insert(aew);
        }
        // cout << "weight: " << weight << "\n";
        ret += weight*weight;
    }
    // cout << "ndoc: " << ndoc << " score: " << sqrt(ret) << "\n";
    pair<float, unordered_map<int, float> > retorna(sqrt(ret), foo);
    return retorna;
}

int queryProcessor::getDomainN(int ndoc){
    string path = "docs/mapPgDom/";
    int nfile = ndoc/1000;
    path.append(to_string(nfile));
    ifstream file;
    file.open(path, ios::in);
    string entry;
    getline(file,entry);
    // cout << "entry: "<< entry << "\n";
    int pos=0;
    while (entry[pos] != ' ') pos++;
    int id = stoi(entry.substr(0,pos));
    int ndocs = stoi(entry.substr(pos+1,entry.size()));
    while ((id != ndoc) && (!file.eof())){
        getline(file,entry);
        pos=0;
        while (entry[pos] != ' ') pos++;
        if (entry.size() > 0){
            id = stoi(entry.substr(0,pos));
            ndocs = stoi(entry.substr(pos+1,entry.size()));
        }
    }
    if (id == ndoc) return ndocs;
    return 0;
}

float queryProcessor::getPageRank(int ndoc){
    ifstream prfile;
    int doc = getDomainN(ndoc);
    if (doc == 0){
        return 0;
    }
    string fileName = "docs/PageRankDom/values/";
    int nfile = doc/1000;
    fileName.append(to_string(nfile));
    prfile.open(fileName, ios::in);
    string entry;
    getline(prfile,entry);
    int pos=0;
    while (entry[pos] != ' ') pos++;
    int id = stoi(entry.substr(0,pos));
    float pr = stof(entry.substr(pos+1,entry.size()));
    while ((id != doc) && (!prfile.eof())){
        getline(prfile,entry);
        pos=0;
        while (entry[pos] != ' ') pos++;
        if (entry.size() > 0){
            id = stoi(entry.substr(0,pos));
            pr = stof(entry.substr(pos+1,entry.size()));
        }
    }
    if (id == doc)    return pr;
    return 0;
    prfile.close();
}

unordered_map<int,float> queryProcessor::getAnchorFreq(vector<string> termos){
    // vector<int> termosID;
    // for (int i=0; i<termos.size(); i++){
    //     termosID.push_back(getAnchorNum(termos[i]));
    // }
    unordered_map<int,float> retorno;
    for (int i=0; i<termos.size(); i++){
        string FileName = "anchorRuns/runinvIndex";
        int termId = getAnchorNum(termos[i]);
        int FileNum = termId/1000;
        FileName.append(to_string(FileNum));
        run r(FileName);
        // vector<int> ret = r.readAndDecode();
        // while ((ret[0] != termId) && (!r.isEOF())){
        //     ret = r.readAndDecode(); //lê o índice até encontrar o termo em questão
        // }
        // r.close();
        vector<int> ret = r.readAndDecode();
        while ((ret[0] != termId) && (!r.isEOF())){
            // cout << "ret1: " << ret[2] << "\n";
            // cout << "term: \n";
            // if (ret.size() == 3){
            // for (vector<int>::iterator it = ret.begin(); it != ret.end(); ++it) cout << *it << " ";
            // cout << "\n";
            // cout <<"retsize: " << ret.size() << "\n";
            // cout << "alo alo\n";
            r.seek(ret[1]);
            // cout << "marciano\n";
            if (!r.isEOF())   ret = r.readAndDecode(); //lê o índice até encontrar o termo em questão
            // cout << "olar\n";
        }
        // cout << "frequencia na coleção: " << ret[2] << " docs in a total of " << ndoctot << "\n";
        if (!r.isEOF()) ret = r.readAndDecode();
        r.close();
        // cout << "term: " << termId <<" \n";
        // vector<int> ret = r.readAndDecode();
        // cout << ret[0] << "\n";
        // while ((ret[0] != termId) && (!r.isEOF())){
        //     // cout << ret[0] << "\n";
        //     // cout << "term: \n";
        //     // if (ret.size() == 3)   for (vector<int>::iterator it = ret.begin(); it != ret.end(); ++it) cout << *it << " ";
        //     // cout << "ret size: " << ret.size() << "\n";
        //     r.seek(ret[1]);
        //     ret = r.readAndDecode(); //lê o índice até encontrar o termo em questão
        // }
        // ret = r.readAndDecode();
        // r.close();
        // vector<int> retorno;
        // // vector<int> retorno;
        // // int curDoc = 0;
        // // for (int i=1; i<ret.size(); i++){
        // //     if (i%2 == 1){
        // //         curDoc += ret[i];
        // //         // cout << "doc: " << curDoc << "\n";
        // //     }
        // //     retorno.push_back(curDoc); //lê todos os documentos da lista
        // // }
        // // if (ret[0] != termId){
        // //     vector<int> empty;
        // //     return empty;
        // // }
        // // return retorno;
        int curDoc = 0;
        for (int i=1; i<ret.size(); i+=2){
            curDoc = ret[i];
            unordered_map<int,float>::iterator it = retorno.find(curDoc);
            if (it == retorno.end()){
                pair<int,float> asd(curDoc,(float) ret[i+1]);
                retorno.insert(asd);
            }
            else{
                it->second += ret[i+1];
            }
            // retorno.push_back(ret[i]); //lê todos os documentos da lista
        }
    }
    return retorno;
}

bool compara(documentoWei a, documentoWei b){
    return (a.score > b.score);
}

bool compara2(pair<float,int> a, pair<float,int> b){
    return (a.first > b.first);
}

unordered_map<int,float> scaleVector(unordered_map<int,float> a, float mult){
    float max = 0;
    for (unordered_map<int,float>::iterator it = a.begin(); it != a.end(); it++){
        if (it->second > max) max = it->second;
    }
    for (unordered_map<int,float>::iterator it = a.begin(); it != a.end(); it++){
        it->second /= max;
        it->second *= mult;
    }
    return a;
}

void queryProcessor::process(string consulta){
    parser pars;
    pars.normalizeText(consulta);
    // cout << "ndoctot: " << ndoctot << "\n";
    vector<string> termos = pars.getTerms(consulta); //normaliza e separa os termos da consulta
    string prev = termos[0];
    vector<int> docss;
    unordered_set<int> quer;
    vector<int> quervec;
    unordered_map<int,float> anchor = getAnchorFreq(termos);
    // cout << "teste1!\n";
    for (int i=0; i<termos.size(); i++){
        string termo = termos[i];
        // cout << "teste1!\n";
        int termId = getTermID(termo);
        // cout << "teste1!\n";
        vector<int> term1 = getDocs(termId);
        // cout << "teste1!\n";
        // cout << "docs: " << term1.size() << "\n";
        for (int j=0;    j<term1.size(); j++){
            unordered_set<int> docss2;
            if (j%2 == 0){
                if (docss2.find(term1[j]) == docss2.end()){
                    docss2.insert(term1[j]);
                }
            }
            // for (unordered_map<int,float>::iterator it=anchor.begin(); it != anchor.end(); it++){
            //     if (docss2.find(it->first) == docss2.end()){
            //         docss2.insert(it->first);
            //     }
            // }
            for (unordered_set<int>::iterator it=docss2.begin(); it!=docss2.end(); it++){
                docss.push_back(*it);
            }
        }
        int tnum = getTermID(termo);
        quervec.push_back(tnum);
        quer.insert(tnum);
    }
    // for (int i=0; i<docss.size(); i++)  cout << findDoc(docss[i]) << "\n";

    // cout << "docs size: " << docss.size() << "\n";

    vector<pair<float, int> > aux;

    unordered_map<int,float> prs;
    for (int i=0; i<docss.size(); i++){
        pair<int,float> asd(docss[i],getPageRank(docss[i]));
        prs.insert(asd);
        // pair<float,int> asd(prs,docss[i]);
        // aux.push_back(asd);
    }

    prs = scaleVector(prs,0.8);
    anchor = scaleVector(anchor,0.5);
    // for (int i=0; i<docss.size(); i++){
    //     float prs = getPageRank(docss[i]);
    //     pair<float,int> asd(prs,docss[i]);
    //     aux.push_back(asd);
    // }

    for (int i=0; i<docss.size(); i++){
        float scoreA = 0, scoreP = 0;
        unordered_map<int,float>::iterator it = prs.find(docss[i]);
        scoreP = it->second;
        it = anchor.find(docss[i]);
        if (it != anchor.end()) scoreA = it->second;
        pair<float,int> asd((scoreA + scoreP),docss[i]);
        aux.push_back(asd);
    }

    sort(aux.begin(), aux.end(), compara2);

    vector<float> prVec;
    vector<int> docsaux;
    for (int i=0; i<aux.size(); i++){
        if (i<50){
            docsaux.push_back(aux[i].second);
            prVec.push_back(aux[i].first);
            // cout << "pr: " << aux[i].first << " doc: " << aux[i].second << "\n";
        }
    }
    // docsaux.push_back(docss[0]);
    docss = docsaux;


    // cout << "auxSize: " << aux.size() << "\n";


    if (docss.size() == 0){
        cout << "Termo nao encontrado!\n";
        return;
    }

    unordered_map<int,float> cosineSim;
    for (int i=0; i<docss.size(); i++){
        // string docName =
        // documentoWei doc1;
        // cout << "searching...\n";
        // float prevScore = o
        // doc1.url = findDoc(docss[i]);
        // cout << "oi\n";
        pair<float, unordered_map<int, float> > docInfo = getWd(docss[i],quer);
        // cout << "got info\n";
        float norma = docInfo.first;
        // doc1.score = 0;
        float cosineScore = 0;
        for (int j=0; j<quervec.size(); j++){
            unordered_map<int,float> aq = docInfo.second;
            unordered_map<int,float>::iterator it = aq.find(quervec[j]);
            if (it != aq.end()){
                // cout << "asd> " << it->second << "\n";
                cosineScore += it->second;
            }
        }
        cosineScore /= norma;
        // doc1.score += prVec[i];
        // docWei.push_back(doc1);
        // cout << "cosine score of " << docss[i] << ": " << findDoc(docss[i]) << ": " << cosineScore << "\n";
        pair<int,float> aux(docss[i],cosineScore);
        cosineSim.insert(aux);
    }

    cosineSim = scaleVector(cosineSim,1);

    vector<documentoWei> docWei;
    for (int i=0; i<docss.size(); i++){
        // string docName =
        documentoWei doc1;
        // cout << "searching...\n";
        // float prevScore = o
        doc1.url = findDoc(docss[i]);
        // cout << "found: " << doc1.url << "\n";
        // pair<float, unordered_map<int, float> > docInfo = getWd(docss[i],quer);
        // cout << "got info\n";
        // doc1.norm = docInfo.first;
        doc1.score = 0;
        unordered_map<int,float>::iterator it = cosineSim.find(docss[i]);
        doc1.score += it->second;
        it = anchor.find(docss[i]);
        if (it != anchor.end()) doc1.score += it->second;
        it = prs.find(docss[i]);
        doc1.score += it->second;
        // for (int j=0; j<quervec.size(); j++){
        //     unordered_map<int,float> aq = docInfo.second;
        //     unordered_map<int,float>::iterator it = aq.find(quervec[j]);
        //     if (it != aq.end()){
        //         doc1.score += it->second;
        //     }
        // }
        // doc1.score /= doc1.norm;
        // cout << "score of " << doc1.url << ": " << doc1.score << "\n";
        // doc1.score += prVec[i];
        docWei.push_back(doc1);
    }

    sort(docWei.begin(), docWei.end(), compara);

    cout << "O termo se encontra nos seguintes documentos:\n";
    int sizeDW = docWei.size();
    int limite = min(10, sizeDW);
    for (int i=0; i<limite; i++){
        // string docToReturn = findDoc(term1[i]);
        cout << docWei[i].url << "\n";// << docWei[i].score << "\n";
    }
    // cout << "aew\n";
    // int sum=0;
    // for (int i=0; i<term1.size(); i++){
    //     term1[i]+=sum;
    //     sum = term1[i];
    // }
    // for (int i=1; i < termos.size(); i++){
    //     vector<int> term2 = getDocs(termos[i]); //processa as listas dos termos, um por um,
    //     // int sum=0;                                                                    //fazendo a interseção à medida que busca.
    //     // for (int k=0; k<term2.size(); k++){
    //     //     term2[k]+=sum;
    //     //     sum = term2[k];
    //     // }
    //     term1 = mergeTerms(term1, term2);
    // }

    return;
}
