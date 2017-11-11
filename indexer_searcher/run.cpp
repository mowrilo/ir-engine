#include "run.hpp"

run::run(string name){
    fileName = name;
    runFile.open(fileName,ios::in | ios::binary);
}

void run::deleteFile(void){ //remove o arquivo
    remove(fileName.c_str());
}

bool run::isOpen(void){
    return runFile.is_open();
}

void run::seek(int nbytes){
    char *c = new char;
    runFile.read(c,1);
    // runFile.unget();
    // cout << "previous byte: " << (int) *c <<"\n";
    int posicao = runFile.tellg();
    // runFile.read(c,1);
    // while (((int) *c) >= 0){
    // }
    // posicao +=nbytes;
    // runFile.seekg(posicao);
    runFile.read(c,1);
    runFile.unget();
    while (((int) *c >= 0) && (!runFile.eof())){
        // cout << "MENOS\n";
        // posicao++;
        // runFile.seekg(posicao);
        runFile.read(c,1);
        // cout << "eof: " << isEOF() << "\n";
    }
    if (!runFile.eof())   runFile.unget();
    // cout << "current byte: " << (int) *c <<"\n";
    // cout << "seeking " << nbytes << " bytes from position " << posicao <<"\n";
    // for (int i=0; i<=nbytes; i++){
    //     posicao++;
    // }
    delete[] c;
}

vector<int> run::readAndDecode(){
    // cout << "runanddecode\n";
    vector<int> ret;
    char *c = new char;
    vector<char> cVec;
    // cout << runFile.eof() << "\n";
    runFile.read(c,1); //o primeiro byte de uma entrada sempre terá o bit mais significativo igual a 1
    char c2 = *c;
    // if ((int) c2 >= 0)    cout << "PORRA VEI!!!\n\n\n";
    cVec.push_back(c2);
    runFile.read(c,1);
    c2=*c;
    int teste = 1;
    // cout << "olarolarolar\n" << "\n";
    while ((((int) c2) >= 0) && (!runFile.eof())){ //lê bytes enquanto o bit mais significativo seja igual a zero.
        cVec.push_back(c2);
        runFile.read(c,1);
        // cout << teste << " " << (int) c2 << " oi\n";
        teste++;
        c2=*c;
    }
    // cout << "saiu\n";
    if ((((int) c2) < 0)) runFile.unget(); //devolve o último
    delete[] c;
    // cout << "real number of bytes: " << cVec.size() << "\n";
    int nums[3], i=0;
    int by = 0;
    char c1 = cVec[0];
    bool keep = true;
    while (keep){
        int pos=0;
        vector<bool> boolvec;
        int aux=i;
        if(((c1 & (1 << aux)) == 0)){
            pos=1;
        }
        else{
            while ((c1 & (1 << i)) != 0){ //caminha bit pot bit, incrementando os bytes ao se chegar
                boolvec.push_back(true);    // no penúltimo bit. este while lê a primeira parte do código elias gamma (unário)
                i++;
                if (i == 7){
                    by++;
                    c1 = cVec[by];
                    i = 0;
                }
                pos++;
            }
        }
        boolvec.push_back(false);
        i++;
        if (i == 7){
            by++;
            c1 = cVec[by];
            i = 0;
        }
        int n=pos;
        for (int count=0; count<n; count++){ //lê a quantidade restante de bits, dependendo do tamanho da primeira parte
            boolvec.push_back((c1 & (1 << i)) != 0);
            i++;
            if (i == 7){
                by++;
                c1 = cVec[by];
                i = 0;
            }
            pos++;
        }
        // cout << "boolvec:\n";
        // for (int asd=0; asd < boolvec.size(); asd++){
        //     cout << boolvec[asd];
        // }
        // cout << "alo alo marciano\n";
        int auxx = 0;
        eliasCoding::decode(&auxx,boolvec); //decodifica o número
        ret.push_back(auxx);
        keep = (by < (cVec.size() - 1));
        // if ((int) cVec[0] == -86){
        //     cout << "\n" << by << " keep: " << (int) keep << "\n";
        // }
        if (by == (cVec.size() - 1)){
            for (int count=i; count < 7; count++){
                if ((cVec[by] & (1 << count)) != 0)    keep = true;
            }
        }
        if ((ret.size() != 1) && (ret.size()%2 == 1)){
            if (i != 0){
                by++;
                c1 = cVec[by]; //caso leia a frequência, incrementa o byte, pois não há mais nada a ser lido neste
                i=0;
            }
        }
    }
    // if (ret.size() != 3){
    //     cout << "OPAAAA size: " << ret.size() << "\n";
    //     for (vector<char>::iterator it=cVec.begin(); it != cVec.end();++it){
    //         cout << (int) *it << " ";
    //     }
    //     cout << "\n";
    //     for (vector<int>::iterator it=ret.begin(); it != ret.end(); it++){
    //         cout << *it << " ";
    //     }
    //     cout << "\n\n";
    // }
    return ret;
}

bool run::isEOF(){
    return runFile.eof();
}

void run::close(){
    runFile.close();
}
