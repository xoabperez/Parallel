#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main() {

  const int N = 4, M = 4;
  int i,j;
  int a[N][M] = { };
#pragma omp parallel num_threads(4) private(j)
  for(j=0;j<N;j++){
    #pragma omp critical
    cout << "j = " << j << "; Thread num = " <<  omp_get_thread_num() << endl;
    #pragma omp for
    for(i=0;i<M;i++){
      #pragma omp critical
      cout << "i = " << i << "; Thread num = " <<  omp_get_thread_num() << endl;
      a[i][j] = a[i][j]+j;
    }
  }
  for(i=0;i<N;i++){
    for(j=0;j<M;j++){
      cout << a[i][j] << " ";  
    }
    cout << endl;
  }

}
