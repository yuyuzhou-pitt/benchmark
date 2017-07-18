#include <string>
#include <cstring>
#include <cstddef>
#include <iostream>
#include <climits>
#include <cstdlib>

#define black 1
#define red 0

using namespace std;

class bst {
private:

    struct node {
        int value;
        bool color;
        node *lefttree, *righttree, *parent;

        node() {
            color = red;
            lefttree = NULL;
            righttree = NULL;
            parent = NULL;
            value = 0;
        }

        node* grandparent() {
            if(parent == NULL){
                return NULL;
            }
            return parent->parent;
        }

        node* uncle() {
            if(grandparent() == NULL) {
                return NULL;
            }
            if(parent == grandparent()->righttree)
                return grandparent()->lefttree;
            else
                return grandparent()->righttree;
        }

        node* sibling() {
            if(parent->lefttree == this)
                return parent->righttree;
            else
                return parent->lefttree;
        }
    };

    void rotate_right(node *p){
        node *gp = p->grandparent();
        node *fa = p->parent;
        node *y = p->righttree;

        fa->lefttree = y;

        if(y != nil)
            y->parent = fa;
        p->righttree = fa;
        fa->parent = p;

        if(root == fa)
            root = p;
        p->parent = gp;

        if(gp != NULL){
            if(gp->lefttree == fa)
                gp->lefttree = p;
            else
                gp->righttree = p;
        }

    }

    void rotate_left(node *p){
        if(p->parent == NULL){
            root = p;
            return;
        }
        node *gp = p->grandparent();
        node *fa = p->parent;
        node *y = p->lefttree;

        fa->righttree = y;

        if(y != nil)
            y->parent = fa;
        p->lefttree = fa;
        fa->parent = p;

        if(root == fa)
            root = p;
        p->parent = gp;

        if(gp != NULL){
            if(gp->lefttree == fa)
                gp->lefttree = p;
            else
                gp->righttree = p;
        }
    }

    void inorder(node *p){
        if(p == nil)
            return;

        if(p->lefttree)
            inorder(p->lefttree);

        cout << p->value << " ";
                
        if(p->righttree)
            inorder(p->righttree);
    }

    string outputcolor (bool color) {
        return color ? "black" : "red";
    }

    node* getsmallestchild(node *p){
        if(p->lefttree == nil)
            return p;
        return getsmallestchild(p->lefttree);
    }

    bool delete_child(node *p, int data){
        if(p->value > data){
            if(p->lefttree == nil){
                return false;
            }
            return delete_child(p->lefttree, data);
        } else if(p->value < data){
            if(p->righttree == nil){
                return false;
            }
            return delete_child(p->righttree, data);
        } else if(p->value == data){
            if(p->righttree == nil){
                delete_one_child (p);
                return true;
            }
            node *smallest = getsmallestchild(p->righttree);
            swap(p->value, smallest->value);
            delete_one_child (smallest);

            return true;
        }
    }

    void delete_one_child(node *p){
        node *child = p->lefttree == nil ? p->righttree : p->lefttree;
        if(p->parent == NULL && p->lefttree == nil && p->righttree == nil){
            p = NULL;
            root = p;
            return;
        }
        
        if(p->parent == NULL){
            delete  p;
            child->parent = NULL;
            root = child;
            root->color = black;
            return;
        }
        
        if(p->parent->lefttree == p){
            p->parent->lefttree = child;
        } else {
            p->parent->righttree = child;
        }
        child->parent = p->parent;

        if(p->color == black){
            if(child->color == red){
                child->color = black;
            } else
                delete_case (child);
        }

        delete p;
    }

    void delete_case(node *p){
        if(p->parent == NULL){
            p->color = black;
            return;
        }
        if(p->sibling()->color == red) {
            p->parent->color = red;
            p->sibling()->color = black;
            if(p == p->parent->lefttree)
                rotate_left(p->sibling());
            else
                rotate_right(p->sibling());
        }
        if(p->parent->color == black && p->sibling()->color == black
                && p->sibling()->lefttree->color == black && p->sibling()->righttree->color == black) {
            p->sibling()->color = red;
            delete_case(p->parent);
        } else if(p->parent->color == red && p->sibling()->color == black
                && p->sibling()->lefttree->color == black && p->sibling()->righttree->color == black) {
            p->sibling()->color = red;
            p->parent->color = black;
        } else {
            if(p->sibling()->color == black) {
                if(p == p->parent->lefttree && p->sibling()->lefttree->color == red
                        && p->sibling()->righttree->color == black) {
                    p->sibling()->color = red;
                    p->sibling()->lefttree->color = black;
                    rotate_right(p->sibling()->lefttree);
                } else if(p == p->parent->righttree && p->sibling()->lefttree->color == black
                        && p->sibling()->righttree->color == red) {
                    p->sibling()->color = red;
                    p->sibling()->righttree->color = black;
                    rotate_left(p->sibling()->righttree);
                }
            }
            p->sibling()->color = p->parent->color;
            p->parent->color = black;
            if(p == p->parent->lefttree){
                p->sibling()->righttree->color = black;
                rotate_left(p->sibling());
            } else {
                p->sibling()->lefttree->color = black;
                rotate_right(p->sibling());
            }
        }
    }

    void insert(node *p, int data){
        if(p->value >= data){
            if(p->lefttree != nil)
                insert(p->lefttree, data);
            else {
                node *tmp = new node();
                tmp->value = data;
                tmp->lefttree = tmp->righttree = nil;
                tmp->parent = p;
                p->lefttree = tmp;
                insert_case (tmp);
            }
        } else {
            if(p->righttree != nil)
                insert(p->righttree, data);
            else {
                node *tmp = new node();
                tmp->value = data;
                tmp->lefttree = tmp->righttree = nil;
                tmp->parent = p;
                p->righttree = tmp;
                insert_case (tmp);
            }
        }
    }

    void insert_case(node *p){
        if(p->parent == NULL){
            root = p;
            p->color = black;
            return;
        }
        if(p->parent->color == red){
            if(p->uncle()->color == red) {
                p->parent->color = p->uncle()->color = black;
                p->grandparent()->color = red;
                insert_case(p->grandparent());
            } else {
                if(p->parent->righttree == p && p->grandparent()->lefttree == p->parent) {
                    rotate_left (p);
                    rotate_right (p);
                    p->color = black;
                    p->lefttree->color = p->righttree->color = red;
                } else if(p->parent->lefttree == p && p->grandparent()->righttree == p->parent) {
                    rotate_right (p);
                    rotate_left (p);
                    p->color = black;
                    p->lefttree->color = p->righttree->color = red;
                } else if(p->parent->lefttree == p && p->grandparent()->lefttree == p->parent) {
                    p->parent->color = black;
                    p->grandparent()->color = red;
                    rotate_right(p->parent);
                } else if(p->parent->righttree == p && p->grandparent()->righttree == p->parent) {
                    p->parent->color = black;
                    p->grandparent()->color = red;
                    rotate_left(p->parent);
                }
            }
        }
    }

    void deletetree(node *p){
        if(!p || p == nil){
            return;
        }
        deletetree(p->lefttree);
        deletetree(p->righttree);
        delete p;
    }
public:

    bst() {
        nil = new node();
        nil->color = black;
        root = NULL;
    }

    ~bst() {
        if (root)
            deletetree (root);
        delete nil;
    }

    void inorder() {
        if(root == NULL)
            return;
        inorder (root);
        cout << endl;
    }

    void insert (int x) {
        if(root == NULL){
            root = new node();
            root->color = black;
            root->lefttree = root->righttree = nil;
            root->value = x;
        } else {
            insert(root, x);
        }
    }

    bool delete_value (int data) {
        return delete_child(root, data);
    }
private:
    node *root, *nil;
};

int idle_loop(long long size){
  long long i;
  for (i=0;i<size;++i){
    __asm__ ("mov %%eax, %%eax"::);
  }
}

int main (int argc, char* argv[])
{
  string usage = "usage: ./rb-tree <insert|delete|list> <size> <rounds> <idle_loop>";

  if ( argc < 4 ){
   cout << usage << endl;
   return -1;
  }

  char* cmd = argv[1];
  long long size = atoll(argv[2]);

  int rounds = 1; // run 1 time by default 
  if ( argc > 3) {
    rounds = atoll(argv[3]);
  }

  long long idle_l = 0; // no idle by default
  if ( argc > 4) {
    idle_l = atoll(argv[4]);
  }

  cout << cmd << " with size " << size << " repeat " << rounds << " rounds" << endl;


  bst rb_tree;
  int range = INT_MAX;

int i;
for (i=0;i<rounds;++i){

  if ( strcmp( cmd, "insert" ) == 0 ) {
    __asm__ ("lfence");
    for (long long i=0;i<size;++i){
     int val = rand() % range;
     rb_tree.insert(val);
    }
    __asm__ ("sfence");
  }
  else if ( strcmp( cmd, "delete" ) == 0 ){
    for (long long i=0;i<size;++i){
     rb_tree.insert(i);
    }
    __asm__ ("lfence");
    for (long long i=0;i<size;++i){
     rb_tree.delete_value(i);
    }
    __asm__ ("sfence");
  }
  else if ( strcmp( cmd, "list" ) == 0 ){
    for (long long i=0;i<size;++i){
     int val = rand() % range;
     rb_tree.insert(val);
    }
    __asm__ ("lfence");
    rb_tree.inorder();
    __asm__ ("sfence");
  }
  else {
    cout << usage << endl;
    return -1;
  }

  idle_loop(idle_l);
  cout << size << " items returned in round " << i << "." << endl;

}

  return 0;
}
