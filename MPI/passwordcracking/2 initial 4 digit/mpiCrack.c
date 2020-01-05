#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <crypt.h>
#include <mpi.h>

/*****************************************************************************
The variable names and the function names of this program is same as provided by the university.
The added variable and function are the only changes made to this program.
 

To compile:
     mpicc -o mpiCrack mpiCrack.c -lcrypt
     
  To run 3 processes on this computer:
    mpirun -n 3 ./mpiCrack
*****************************************************************************/


int n_passwords = 4;

char *encrypted_passwords[] = {

 "$6$KB$tAjstsBV1XsDRliMb1FTBSXPLtf7bhDi6sH3TA8kko/1NXNdTNlLV2WsUq8/OkbNDkqhur9CBtf7LcqlQQQ1u.",
  "$6$KB$z.Sy9Y2zNOiYld6XvxldsDOcOwMd96QmgqPKEqoQBv1QpV9RterSq7pGpqWJZ4/95mBXz7/9XdER4.YUGqR6a1",
  "$6$KB$leoF20RqFnb1WExjyeQjd4.lyH2rFuTZ2IpuLPQqIo3Fjn3f0a6l6og.kUf2iXzxmDiuvYVdF3TjoDX4aqXuw1",
  "$6$KB$pbILEjhvuaULmXRUXJseErniIBhMyT6hg95krsB9vewYKxHjkeiEJx7wltc3rT/UmJGrJEAAJBO8vvxEXCf2t1"
};


void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}
 
void f1(char *salt_and_encrypted){
  int b, c, d1, d2;    
  char salt[7];    

  char plain[7];  
  char *enc;      
  int count = 0;  

  substr(salt, salt_and_encrypted, 0, 6);
 
  for(b='A'; b<='M';b++){
    for(c='A'; c<='Z'; c++){
      for(d1=0; d1<=99; d1++){
for(d2=0; d2<=99; d2++){
//printf("Instance 1:");
sprintf(plain, "%c%c%02d%02d",b, c, d1, d2);
enc = (char *) crypt(plain, salt);
count++;
if(strcmp(salt_and_encrypted, enc) == 0){
 printf("#%-8d%s %s\n", count, plain, enc);
} 
}
      }
    }
  }
  printf("%d solutions explored\n", count);
}
void f2(char *salt_and_encrypted){
  int b, c, d1, d2;    
  char salt[7];    

  char plain[7];  
  char *enc;      
  int count = 0;  

  substr(salt, salt_and_encrypted, 0, 6);
 
  for(b='N'; b<='Z'; b++){
    for(c='A'; c<='Z'; c++){
      for(d1=0; d1<=99; d1++){
for(d2=0; d2<=99; d2++){
//printf("Instance 2:");
sprintf(plain, "%c%c%02d%02d",b, c, d1, d2);
enc = (char *) crypt(plain, salt);
count++;
if(strcmp(salt_and_encrypted, enc) == 0){
 printf("#%-8d%s %s\n", count, plain, enc);
} 
}
      }
    }
  }
  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec;
  long long int dn =  finish->tv_nsec - start->tv_nsec;

  if(dn < 0 ) {
    ds--;
    dn += 1000000000;
  }
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(int argc, char** argv) {
 struct timespec start, finish;  
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

 
  int size, rank;
int i;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {
    if(rank ==0){

      int x;
   
      MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Send(&x, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);


 
    } else if(rank==1){
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
        for(i=0;i<n_passwords;i<i++) {
          f1(encrypted_passwords[i]);
        }
      }
      else{
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
        for(i=0;i<n_passwords;i<i++) {
          f2(encrypted_passwords[i]);
        }
      }
    }
    MPI_Finalize();
 clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
if(rank==0){
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed/1.0e9));
}
  return 0;
}  
