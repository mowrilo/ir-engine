#include <iostream>
#include <deque>
#include "url.hpp"

using namespace std;

class binHeap {
  public:
        binHeap(int max);
        void heapify();
        void insert(url a);
        url extractMin();

  private:
        deque<url> queue;
        int maxSize;
}
