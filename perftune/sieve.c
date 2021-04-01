#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
//Sieve of Atkin 

#define N 10000000

static bool is_prime[N];
static int  primes[N];
int num=0;

int *sieve(int n) {
  assert(n + 1 < N);
  // for (int i = 0; i <= n; i++)
  //   is_prime[i] = true;

  // for (int i = 2; i <= n; i++) {
  //   for (int j = i + i; j <= n; j += i) {
  //     is_prime[j] = false;
  //   }
  // }

  // int *p = primes;
  // for (int i = 2; i <= n; i++)
  //   if (is_prime[i]) {
  //     *p++ = i;
  //   }
  // *p = 0;
  is_prime[0]=is_prime[1]=1;
  is_prime[2]=0;
  primes[num++]=2;
  for(int i=3;i<=n;i+=2){
    if(!is_prime[i])  primes[num++]=i;
    for(int j=1;j<num;j++){
      if(i*primes[j]>n) break;
      is_prime[i*primes[j]]=1;
      if(i%primes[j]==0)  break;
    }
  }
  primes[num]=0;
  return primes;
}
