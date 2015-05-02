/* { dg-do compile } */
/* { dg-options "--param allow-store-data-races=0 -O2 -fdump-tree-lim1" } */

/* Test that `count' is not written to unless p->data > 0.  */

int count;

struct obj {
    int data;
    struct obj *next;
} *q;

void func()
{
  struct obj *p;
  for (p = q; p; p = p->next)
    if (p->data > 0)
      count++;
}

/* { dg-final { scan-tree-dump-times "MEM count_lsm.. count_lsm_flag" 1 "lim1" } } */
/* { dg-final { cleanup-tree-dump "lim1" } } */
