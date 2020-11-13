#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TLB_ENTRIES 16
//global page size variable

typedef struct{
  int tag;
  int physicalPageNumber;
} tlbObject;


int main(int argc, char *argv[]){
  tlbObject *tlb;
  tlb = malloc(TLB_ENTRIES);

  FILE *fp;
  int numAddresses;
  char buffer[100000]; //contents in addresses.txt

  if (argc != 2) {
       printf("Usage: %s <addresses.txt> \n", argv[0]);
       return -1;
   }

  char* fileName = argv[1];
  /* Open file for both reading */
  int count=0;
  fp = fopen(fileName, "r");
  if (fp == NULL){
    fprintf((stderr), "File didn't open :( \n" );
    exit(1);
  }
  numAddresses = fread(buffer,sizeof(char),sizeof(buffer),fp);
  printf("number of items read=  %d\n", numAddresses);
  //printf( "Contents of buffer = %.100000s\n", buffer );
  printf("%s",buffer);
  fclose(fp);

  //convert int to binary for offset calc

   //read addresses .txt, use fgets()
    //translate each address and translate it to its corresponding physical addresses

   //read binary file/backing_store.bin binary file
      //use fopen(),fread(),fseek(), and fclose()

  return (0);
   //output the value of the signed byte at the physical address

}
