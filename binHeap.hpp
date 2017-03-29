#include <iostream>
#include <deque>
#include "url.hpp"

using namespace std;

class binHeap {
  public:
        void insert(url a);
        url extractMin();
        void heapify();

  private:
        deque<url> queue;
        int maxSize;

}
