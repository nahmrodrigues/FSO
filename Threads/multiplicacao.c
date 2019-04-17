#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int m1[1000][1000],m2[1000][1000];
int m3[1000][1000];

struct t_param
{
  int inicio;
  int fim;
  int size;
};

void *multiplica(void *l)
{
  for(int i=((struct t_param*)l)->inicio;i<((struct t_param*)l)->fim;i++)
    for(int j=0;j<((struct t_param*)l)->size;j++)
    {
      for(int k=0;k<((struct t_param*)l)->size;k++)
        m3[i][j]+=m1[i][k]*m2[k][j];
    }
  int r=0;
  pthread_exit((void *)&r);
}

int main(void)
{
  int size;
  scanf("%d",&size);
  for(int i=0;i<size;i++)
    for(int j=0;j<size;j++)
    {
      scanf("%d",&m1[i][j]);
      m3[i][j]=0;
    }
  for(int i=0;i<size;i++)
    for(int j=0;j<size;j++)
      scanf("%d",&m2[i][j]);

  struct t_param t;
  t.inicio=0;
  t.fim=2;
  t.size=size;
  int threads=4;
  int divisao=size/threads;
  int resto=size%threads;
  pthread_t ids[threads];
  for(int i=0;i<threads;i++)
  {
    struct t_param *t;
    t=malloc(sizeof(struct t_param));
    t->inicio=i*divisao;
    if(i==threads-1)
      t->fim=resto+i*divisao+divisao;
    else
      t->fim=i*divisao+divisao;
    t->size=size;
    fprintf(stderr,"--> %d %d\n",t->inicio,t->fim);
    pthread_create(&ids[i],NULL, multiplica,(void*)t);
  }

  fprintf(stderr,"aguardando threads");
  for(int i=0;i<threads;i++)
  {
    pthread_join(ids[i],NULL);
    fprintf(stderr,".");
  }
  fprintf(stderr,"done.\n");

  for(int i=0;i<size;i++)
  {
    printf("%d ",m3[i][0]);
    for(int j=1;j<size;j++)
      printf(" %d",m3[i][j]);
    printf("\n");
  }
  return 0;
}
