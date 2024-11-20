// C program for Bplus tree
#include <stdio.h>
#include <stdlib.h>
struct BplusTree {
   int *d;
   struct BplusTree **child_ptr;
   int l;
   int n;
};
struct BplusTree *r = NULL, *np = NULL, *x = NULL;
struct BplusTree* init() {
    //to create nodes
   int i;
   np = (struct BplusTree*)malloc(sizeof(struct BplusTree));
   np->d = (int*)malloc(2 * sizeof(int)); // order 6
   np->child_ptr = (struct BplusTree**)malloc(3 * sizeof(struct BplusTree*));
   np->l = 1;
   np->n = 0;
   for (i = 0; i < 3; i++) {
      np->child_ptr[i] = NULL;
   }
   return np;
}
void traverse(struct BplusTree *p) {
    //traverse tree
   printf("\n");
   int i;
   for (i = 0; i < p->n; i++) {
      if (p->l == 0) {
         traverse(p->child_ptr[i]);
      }
      printf(" %d", p->d[i]);
   }
   if (p->l == 0) {
      traverse(p->child_ptr[i]);
   }
   printf("\n");
}

void sort(int *p, int n) {
   int i, j, t;
   for (i = 0; i < n; i++) {
      for (j = i; j <= n; j++) {
         if (p[i] > p[j]) {
            t = p[i];
            p[i] = p[j];
            p[j] = t;
         }
      }
   }
}

int split_child(struct BplusTree *x, int i) {
   int j, mid;
   struct BplusTree *np1, *np3, *y;
   np3 = init();
   np3->l = 1;
   if (i == -1) {
      mid = x->d[1];
      x->d[1] = 0;
      x->n--;
      np1 = init();
      np1->l = 0;
      x->l = 1;
      for (j = 2; j < 3; j++) {
         np3->d[j - 2] = x->d[j];
         np3->child_ptr[j - 2] = x->child_ptr[j];
         np3->n++;
         x->d[j] = 0;
         x->n--;
      }
      for (j = 0; j < 3; j++) {
         x->child_ptr[j] = NULL;
      }
      np1->d[0] = mid;
      np1->child_ptr[np1->n] = x;
      np1->child_ptr[np1->n + 1] = np3;
      np1->n++;
      r = np1;
   } else {
      y = x->child_ptr[i];
      mid = y->d[1];
      y->d[1] = 0;
      y->n--;
      for (j = 2; j < 3; j++) {
         np3->d[j - 2] = y->d[j];
         np3->n++;
         y->d[j] = 0;
         y->n--;
      }
      x->child_ptr[i + 1] = y;
      x->child_ptr[i + 1] = np3;
   }
   return mid;
}

void insert(int a) {
   int i, t;
   x = r;
   if (x == NULL) {
      r = init();
      x = r;
   } else {
      if (x->l == 1 && x->n == 2) {
         t = split_child(x, -1);
         x = r;
         for (i = 0; i < x->n; i++) {
            if (a > x->d[i] && a < x->d[i + 1]) {
               i++;
               break;
            } else if (a < x->d[0]) {
               break;
            } else {
               continue;
            }
         }
         x = x->child_ptr[i];
      } else {
         while (x->l == 0) {
            for (i = 0; i < x->n; i++) {
               if (a > x->d[i] && a < x->d[i + 1]) {
                  i++;
                  break;
               } else if (a < x->d[0]) {
                  break;
               } else {
                  continue;
               }
            }
            if (x->child_ptr[i]->n == 2) {
               t = split_child(x, i);
               x->d[x->n] = t;
               x->n++;
               continue;
            } else {
               x = x->child_ptr[i];
            }
         }
      }
   }
   x->d[x->n] = a;
   sort(x->d, x->n);
   x->n++;
}

int main() {
   int i, n, t;
   insert(10);
   insert(20);
   insert(30);
   insert(40);
   insert(50);
   printf("Insertion Done");
   printf("\nB+ tree:");
   traverse(r);
   return 0;
}