#include <iostream>
#include <stdlib.h>
#include <memory>
#include <cstring>
#include <limits.h>

using namespace std;
const int MAX = 4 ;
const int MIN = 2 ;
struct btnode
{
	int count ;
	long long value[MAX + 1] ;
	btnode *child[MAX + 1] ;
} ;
class BTree
{
	private :
		btnode *root ;
	public :
		BTree( ) ;
		void insert ( long long val ) ;
		int setval ( long long val, btnode *n, int *p, btnode **c ) ;
		static btnode * search ( long long val, btnode *root, int *pos ) ;
		static int searchnode ( long long val, btnode *n, int *pos ) ;
		void fillnode ( long long val, btnode *c, btnode *n, int k ) ;
		void split ( long long val, btnode *c, btnode *n,
				int k, int *y, btnode **newnode ) ;
		void del ( long long val ) ;
		int delhelp ( long long val, btnode *root ) ;
		void clear ( btnode *root, int k ) ;
		void copysucc ( btnode *root, int i ) ;
		void restore ( btnode *root, int i ) ;
		void rightshift ( int k ) ;
		void leftshift ( int k ) ;
		void merge ( int k ) ;
		void show( ) ;
		static void display ( btnode *root ) ;
		static void deltree ( btnode *root ) ;
		~BTree( ) ;
} ;
 
BTree :: BTree( )
{
	root = NULL ;
}
void BTree :: insert ( long long val )
{
	int i ;
	btnode *c, *n ;
	int flag ;
	flag = setval ( val, root, &i, &c ) ;
	if ( flag )
	{
		n = new btnode ;
		n -> count = 1 ;
		n -> value[1] = i ;
		n -> child[0] = root ;
		n -> child[1] = c ;
		root = n ;
	}
}
int BTree :: setval ( long long val, btnode *n, int *p, btnode **c )
{
	int k ;
	if ( n == NULL )
	{
		*p = val ;
		*c = NULL ;
		return 1 ;
	}
	else
	{
		if ( searchnode ( val, n, &k ) )
			cout << endl << "Key value already exists." << endl ;
		if ( setval ( val, n -> child[k], p, c ) )
		{
			if ( n -> count < MAX )
			{
				fillnode ( *p, *c, n, k ) ;
				return 0 ;
			}
			else
			{
				split ( *p, *c, n, k, p, c ) ;
				return 1 ;
			}
		}
		return 0 ;
	}
}
btnode * BTree :: search ( long long val, btnode *root, int *pos )
{
	if ( root == NULL )
		return NULL ;
	else
	{
		if ( searchnode ( val, root, pos ) )
			return root ;
		else
			return search ( val, root -> child[*pos], pos ) ;
	}
}
int BTree :: searchnode ( long long val, btnode *n, int *pos )
{
	if ( val < n -> value[1] )
	{
		*pos = 0 ;
		return 0 ;
	}
	else
	{
		*pos = n -> count ;
		while ( ( val < n -> value[*pos] ) && *pos > 1 )
			( *pos )-- ;
		if ( val == n -> value[*pos] )
			return 1 ;
		else
			return 0 ;
	}
}
void BTree :: fillnode ( long long val, btnode *c, btnode *n, int k )
{
	int i ;
	for ( i = n -> count ; i > k ; i-- )
	{
		n -> value[i + 1] = n -> value[i] ;
		n -> child[i + 1] = n -> child[i] ;
	}
	n -> value[k + 1] = val ;
	n -> child[k + 1] = c ;
	n -> count++ ;
}
void BTree :: split ( long long val, btnode *c, btnode *n,
		int k, int *y, btnode **newnode )
{
	int i, mid ;
 
	if ( k <= MIN )
		mid = MIN ;
	else
		mid = MIN + 1 ;
 
	*newnode = new btnode ;
 
	for ( i = mid + 1 ; i <= MAX ; i++ )
	{
		( *newnode ) -> value[i - mid] = n -> value[i] ;
		( *newnode ) -> child[i - mid] = n -> child[i] ;
	}
 
	( *newnode ) -> count = MAX - mid ;
	n -> count = mid ;
 
	if ( k <= MIN )
		fillnode ( val, c, n, k ) ;
	else
		fillnode ( val, c, *newnode, k - mid ) ;
 
	*y = n -> value[n -> count] ;
	( *newnode ) -> child[0] = n -> child[n -> count] ;
	n -> count-- ;
}
void BTree :: del ( long long val )
{
	btnode * temp ;
 
	if ( ! delhelp ( val, root ) )
		cout << endl << "Value " << val << " not found." ;
	else
	{
		if ( root -> count == 0 )
		{
			temp = root ;
			root = root -> child[0] ;
			delete temp ;
		}
	}
}
int BTree :: delhelp ( long long val, btnode *root )
{
	int i ;
	int flag ;
 
	if ( root == NULL )
		return 0 ;
	else
	{
		flag = searchnode ( val, root, &i ) ;
		if ( flag )
		{
			if ( root -> child[i - 1] )
			{
				copysucc ( root, i ) ;
				flag = delhelp ( root -> value[i], root -> child[i] ) ;
				if ( !flag )
					cout << endl << "Value " << val << " not found." ;
			}
			else
				clear ( root, i ) ;
		}
		else
			flag = delhelp ( val, root -> child[i] ) ;
		if ( root -> child[i] != NULL ) {
			if ( root -> child[i] -> count < MIN ) {
				restore ( root, i ) ;
                        }
		}
		return flag ;
	}
}
void BTree :: clear ( btnode *root, int k )
{
	int i ;
	for ( i = k + 1 ; i <= root -> count ; i++ )
	{
		root -> value[i - 1] = root -> value[i] ;
		root -> child[i - 1] = root -> child[i] ;
	}
	root -> count-- ;
}
void BTree :: copysucc ( btnode *root, int i )
{
	btnode *temp = root -> child[i] ;
 
	while ( temp -> child[0] )
		temp = temp -> child[0] ;
 
	root -> value[i] = temp -> value[1] ;
}
void BTree :: restore ( btnode *root, int i )
{
	if ( i == 0 )
	{
		if ( root -> child [1] -> count > MIN )
			leftshift ( 1 ) ;
		else
			merge ( 1 ) ;
	}
	else
	{
		if ( i == root -> count )
		{
			if ( root -> child[i - 1] -> count > MIN )
				rightshift ( i ) ;
			else
				merge ( i ) ;
		}
		else
		{
			if ( root -> child[i - 1] -> count > MIN )
				rightshift ( i ) ;
			else
			{
				if ( root -> child[i + 1] -> count > MIN )
					leftshift ( i + 1 ) ;
				else
					merge ( i ) ;
			}
		}
	}
}
void BTree :: rightshift ( int k )
{
	int i ;
	btnode *temp ;
 
	temp = root -> child[k] ;
 
	for ( i = temp -> count ; i > 0 ; i-- )
	{
		temp -> value[i + 1] = temp -> value[i] ;
		temp -> child[i + 1] = temp -> child[i] ;
	}
 
	temp -> child[1] = temp -> child[0] ;
	temp -> count++ ;
	temp -> value[1] = root -> value[k] ;
	temp = root -> child[k - 1] ;
	root -> value[k] = temp -> value[temp -> count] ;
	root -> child[k] -> child [0] = temp -> child[temp -> count] ;
	temp -> count-- ;
}
void BTree :: leftshift ( int k )
{
	btnode *temp ;
 
	temp = root -> child[k - 1] ;
	temp -> count++ ;
	temp -> value[temp -> count] = root -> value[k] ;
	temp -> child[temp -> count] = root -> child[k] -> child[0] ;
	temp = root -> child[k] ;
	root -> value[k] = temp -> value[1] ;
	temp -> child[0] = temp -> child[1] ;
	temp -> count-- ;
	for ( int i = 1 ; i <= temp -> count ; i++ )
	{
		temp -> value[i] = temp -> value[i + 1] ;
		temp -> child[i] = temp -> child[i + 1] ;
	}
}
void BTree :: merge ( int k )
{
	btnode *temp1, *temp2 ;
	temp1 = root -> child[k] ;
	temp2 = root -> child[k - 1] ;
	temp2 -> count++ ;
	temp2 -> value[temp2 -> count] = root -> value[k] ;
	temp2 -> child[temp2 -> count] = root -> child[0] ;
	for ( int i = 1 ; i <= temp1 -> count ; i++ )
	{
		temp2 -> count++ ;
		temp2 -> value[temp2 -> count] = temp1 -> value[i] ;
		temp2 -> child[temp2 -> count] = temp1 -> child[i] ;
	}
	for ( int i = k ; i < root -> count ; i++ )
	{
		root -> value[i] = root -> value[i + 1] ;
		root -> child[i] = root -> child[i + 1] ;
	}
	root -> count-- ;
	delete temp1 ;
}
void BTree :: show( )
{
	display ( root ) ;
}
void BTree :: display ( btnode *root )
{
	if ( root != NULL )
	{
                int i;
		for ( i = 0 ; i < root -> count ; i++ )
		{
			display ( root -> child[i] ) ;
			cout << root -> value[i + 1] << "\t" ;
		}
		display ( root -> child[i] ) ;
	}
}
void BTree :: deltree ( btnode *root )
{
	if ( root != NULL )
	{
                int i;
		for ( i = 0 ; i < root -> count ; i++ )
		{
			deltree ( root -> child[i] ) ;
			delete ( root -> child[i] ) ;
		}
		deltree ( root -> child[i] ) ;
		delete ( root -> child[i] ) ;
	}
}
 
BTree :: ~BTree( )
{
	deltree ( root ) ;
}
 
int main (int argc, char* argv[])
{

  string usage = "USAGE: ./RB-Tree <insert|delete|list> <size>";

  if ( argc < 3 ){
   cout << usage << endl;
   return -1;
  }

  char* cmd = argv[1];
  long long size = atoll(argv[2]);

  cout << cmd << " with size " << size << endl;

  BTree btree;
  long long range = LLONG_MAX;

  if ( strcmp(cmd, "insert") == 0 ) {
    for (long long i=0;i<size;++i){
     btree.insert(i);
    }
  }
  else if ( strcmp( cmd, "delete" ) == 0 ){
    for (long long i=0;i<size;++i){
     btree.insert(i);
    }
    btree.show();
    for (long long i=size-1;i>=0;--i){
     btree.del(i);
    }
  }
  else if ( strcmp( cmd, "list" ) == 0 ){
    for (long long i=0;i<size;++i){
     btree.insert(i);
    }
    btree.show();
  }
  else {
    cout << usage << endl;
    return -1;
  }

  cout << endl;
  cout << size << " items returned." << endl;

  return 0;
}
