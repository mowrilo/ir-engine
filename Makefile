test:
	export LD_LIBRARY_PATH=./chilkat/lib:$LD_LIBRARY_PATH
	g++ -std=c++11 -L./chilkat/lib -I./chilkat/include url.cpp scheduler.cpp crawler.cpp main.cpp -o crawler -lchilkat-9.5.0 -lresolv -lpthread
