#include <stdint.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void swap(uint64_t &x,uint64_t &y)                        //To swap 2 numbers
{
    uint64_t temp = x;
    x = y;
    y = temp;
}

uint64_t random_range(uint64_t x,uint64_t y)                   //To generate a random number
{
    srand((uint64_t)time(0));
    return (x + (rand() % (uint64_t)(y - x + 1)));
}

int main(int argc, char** argv)
{
    uint64_t n;
    if (argc < 2) {
        cout<<"usage: "<<argv[0]<<" <array_size> <rounds>\n";
        return -1; 
    }
    else {
         n = atoi(argv[1]);
    }

  int rounds= 1; // run 1 time by default
  if ( argc > 2) {
    rounds = atoll(argv[2]);
  }

  cout << argv[0] << " with size " << n << " repeat " << rounds << " rounds" << endl;

  int round;
  for (round=0;round<rounds;++round){
    uint64_t* arr;
    arr = (uint64_t*) calloc(n, sizeof(uint64_t));
    if (arr == NULL) {
         cout <<"Failed to alloc memory\n";
         exit(1);
    }
    for(register uint64_t i = 0;i < n;i++)
        arr[i]=i;

    //shuffle(arr,n);
    __asm__ ("lfence");
    for(register uint64_t i = 0;i < 1000;i++)
        swap(arr[i],arr[random_range(0,n-1)]);
    __asm__ ("sfence");

    cout << n << " times returned in round " << round << "." << endl;
  }
    //display(arr,n);
    //for(uint64_t i = 0;i < n;i++)
    //    cout << arr[i] << ' ';
    //cout << endl;
    return 0;
}
