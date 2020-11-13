#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TLB_ENTRIES 16
//global page size variable

struct{
  int tag;
  int physicalPageNumber;
} tblObject;


int main(int argc, char *argv[]){

  if (argc != 2) {
       printf("Usage: %s <addresses.txt> \n", argv[0]);
       return -1;
   }


   //read addresses .txt, use fgets()
    //translate each address and translate it to its corresponding physical addresses



   //read binary file/backing_store.bin binary file
      //use fopen(),fread(),fseek(), and fclose()


    tblObject tlb = tblObject[TLB_ENTRIES];

   //output the value of the signed byte at the physical address

}
