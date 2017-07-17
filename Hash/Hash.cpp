/* g++  -std=c++0x -o Hash Hash.cpp */
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unordered_map>

using namespace std;
int insert_hash(unordered_map< string, double >& myrecipe, long long size){
  double max = 100.0;
  double min = 0.0;

  long long i;
  for (i=0;i<size;++i){
   string food = "food" + to_string(i);
   double price = (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
   myrecipe.insert (make_pair(food, price)); // move insertion
  }

  return i;
}

int delete_hash(unordered_map< string, double >& myrecipe, long long size){
  for ( auto it = myrecipe.cbegin(); it != myrecipe.cend(); ){
    myrecipe.erase(it++);
  }

  cout << endl;
  return size;
}

int list_hash(unordered_map< string, double >& myrecipe, long long size){
  for (auto& x: myrecipe){
    cout << x.first << ": " << x.second << endl;
  }

  cout << endl;
  return size;
}

int main (int argc, char* argv[])
{

  string usage = "USAGE: ./hash <insert|delete|list> <size> <rounds>";

  if ( argc < 3 ){
   cout << usage << endl;
   return -1;
  }

  char* cmd = argv[1];
  long long size = atoll(argv[2]);

  int rounds = 1; // run 1 time by default 
  if ( argc > 3) {
    rounds = atoll(argv[3]);
  }

  cout << cmd << " with size " << size << " repeat " << rounds << " rounds" << endl;

  unordered_map< string, double > myrecipe;

  long long  ret = 0;

int i;
for (i=0;i<rounds;++i){

  if ( strcmp(cmd, "insert") == 0 ) {
    __asm__ ("lfence");
    ret = insert_hash( myrecipe, size );
    __asm__ ("rfence");
  }
  else 
  if ( strcmp( cmd, "delete" ) == 0 ){
    ret = insert_hash( myrecipe, size );
    __asm__ ("lfence");
    ret = delete_hash( myrecipe, size );
    __asm__ ("rfence");
  }
  else if ( strcmp( cmd, "list" ) == 0 ){
    ret = insert_hash( myrecipe, size );
    __asm__ ("lfence");
    ret = list_hash( myrecipe, size );
    __asm__ ("rfence");
  }
  else {
    cout << usage << endl;
    return -1;
  }
  
  cout << size << " items returned in round " << i << "." << endl;


}

  return 0;
}
