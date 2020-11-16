#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TLB_ENTRIES 16
#define PAGE_SIZE 256  //page size of 256 bytes
#define PAGES 256  //pages begin at 0-255
#define MEMORY_SIZE PAGES * PAGE_SIZE  //bin file size of 65,536 bytes
#define OFFSET 8   //offset bits

#define PAGE_MASK 255  //see page table
#define OFFSET_MASK 255 //0-255 see physical mem table



typedef struct{
  //we're ints before
  unsigned char logicalPageNumber;
  unsigned char  physicalPageNumber;
} tlbObject;

tlbObject *tlb;

int searchTlbTable(int logicalAddress){
  for(int i=0; i<TLB_ENTRIES;i++){
    if(tlb[i].logicalPageNumber==logicalAddress)
      return tlb[i].physicalPageNumber;
  }
  return -1;
}

int addToTable(int logicalAddress, int physicalAddress){
  for(int i=0;i<TLB_ENTRIES;i++){
    if(tlb[i].logicalPageNumber==0){
      tlb[i].logicalPageNumber=logicalAddress;
      tlb[i].physicalPageNumber=physicalAddress;
      return 0;
    }
  }
  for(int i=1;i<TLB_ENTRIES;i++){
    tlb[i-1].logicalPageNumber=tlb[i].logicalPageNumber;
    tlb[i-1].physicalPageNumber=tlb[i].physicalPageNumber;
  }


  tlb[TLB_ENTRIES-1].logicalPageNumber=logicalAddress;
  tlb[TLB_ENTRIES-1].physicalPageNumber=physicalAddress;
  return 0;
}


int main(int argc, char *argv[]){

  tlb = malloc(TLB_ENTRIES);
  printf("%d",tlb[0].logicalPageNumber);

  FILE *fp;
  int numAddresses = 0; //total num addresses
  char buffer[6830]; //contents in addresses.txt

  int logicalAddresses; //the int value being read from addresses.txt

  int pageFaults; //page fault rate = % of address references -> page faults
  int tlbHits; //TLB hit rate = % of addreses references that were resolved in the TLB


  if (argc != 2) {
       printf("Usage: %s <addresses.txt> \n", argv[0]);
       return -1;
   }

   //read addresses.txt
  char* fileName = argv[1];
  fp = fopen(fileName, "r");
  if (fp == NULL){
    fprintf((stderr), "File didn't open :( \n" );
    exit(1);
  }


  // numAddresses = fread(buffer,sizeof(char),sizeof(buffer),fp);
  //sprintf("number of items read=  %d\n", numAddresses);  6831

  //printf( "Contents of buffer = %.100000s\n", buffer );
  // printf("%c\n",buffer[0]);

//prints the buffer/addresses
  int count = 0;
  // for(int i=0;i<sizeof(buffer);i++){
  //   printf("buffer: %c\n",buffer[i]);
  //   printf("buffer atoi:%d \n", atoi(&buffer[i]));
  //   printf("buffer elem size: %lu \n",sizeof(atoi(&buffer[i])));
  //   if(buffer[i]=='\n'){
  //     printf("hi\n");
  // }

  //
  //   // numAddresses++;
  //   // int logicalAddress = atoi(&buffer[i]);
  //   // printf("logical address: %d\n",logicalAddress);
  //   // int offset = logicalAddress & OFFSET_MASK;
  //   // printf("offset: %d\n",offset);
  //   // int logicalPage = (logicalAddress >> OFFSET) & PAGE_MASK;
  //   // printf("logical page: %d\n", logicalPage);
  //   if(count==10){
  //     break;
  //   }
  //   count++;
  // //
  // }
  int *pageTable;
  pageTable = malloc(PAGES);
  printf("size: %lu\n",sizeof(pageTable));
  for(int i = 0; i<sizeof(pageTable);i++){
    printf("%d",pageTable[i]);
  }


  while(fgets(buffer,6830,fp)!=NULL){
    printf("%s",buffer);
    numAddresses++;
    int logicalAddress = atoi(buffer);
    //printf("logical address: %d\n",logicalAddress);
    int offset = logicalAddress % 256;
  //  printf("offset: %s\n",offset);
    int logicalPage = logicalAddress/256;
    //printf("logical page: %s\n", logicalPage);
    int physicalPage=searchTlbTable(logicalPage);
    if(physicalPage !=-1){
      tlbHits++;
    }
    else{
      //calculate physicalPage

      addToTable(logicalPage, physicalPage);
    }

    printf("\n ");
    if(count==5){
      break;
    }
    count++;
  }

  fclose(fp);     //close file








  //convert int to binary for offset calc

   //read addresses .txt, use fgets()
    //translate each address and translate it to its corresponding physical addresses

   //read binary file/backing_store.bin binary file
      //use fopen(),fread(),fseek(), and fclose()

  return (0);
   //output the value of the signed byte at the physical address

}
