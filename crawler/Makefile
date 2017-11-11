main: crawler.o scheduler.o url.o filemanager.o
	g++ -std=c++11 -L./chilkat/lib -I./chilkat/include main.cpp crawler.o scheduler.o url.o filemanager.o -o crawler -lchilkat-9.5.0 -lresolv -lpthread -O2
crawler.o:
	g++ -std=c++11 -L./chilkat/lib -I./chilkat/include -c crawler.cpp -lchilkat-9.5.0 -lresolv -lpthread -O2
scheduler.o:
	g++ -std=c++11 -L./chilkat/lib -I./chilkat/include -c scheduler.cpp -lchilkat-9.5.0 -lresolv -lpthread -O2
url.o:
	g++ -std=c++11 -L./chilkat/lib -I./chilkat/include -c url.cpp -lchilkat-9.5.0 -lresolv -lpthread -O2
filemanager.o:
	g++ -std=c++11 -L./chilkat/lib -I./chilkat/include -c filemanager.cpp -lchilkat-9.5.0 -lresolv -lpthread -O2
