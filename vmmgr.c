//Gabriela Pinto and Katherine Hansen
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


#define TLB_ENTRIES 16
#define PAGE_SIZE 256  //page size of 256 bytes
#define PAGES 256  //pages begin at 0-255
#define MEMORY_SIZE PAGES * PAGE_SIZE  //bin file size of 65,536 bytes
#define OFFSET 8   //offset bits
#define BUFFER_SIZE 16
#define PAGE_MASK 255  //see page table
#define OFFSET_MASK 255 //0-255 see physical mem table



typedef struct{
  int logicalPageNumber;
  int  physicalPageNumber;
} tlbObject;

tlbObject tlb[TLB_ENTRIES];
int tlbIdx = 0;

int pageTable[PAGES];  //page table

char *backingChars; //point to store file

char mainMem[MEMORY_SIZE];

int max(int x,int y){
  if(x>y){
    return x;
  }
  return y;
}
//function to search tlbIdx
//returns -1 if not in table
int searchTlbTable(int logicalPage){
  for(int i = max((tlbIdx-TLB_ENTRIES),0); i<tlbIdx;i++){
    tlbObject entry = tlb[i%TLB_ENTRIES];
    if(entry.logicalPageNumber == logicalPage){
      return entry.physicalPageNumber;
    }
  }
  return -1;
}
//adds tlbObject to the tlb using FIFO
void addToTlb(int logicalPage, int physicalPage){
  tlbObject *newTblObject = &tlb[tlbIdx % TLB_ENTRIES];
  tlbIdx++;
  newTblObject->logicalPageNumber = logicalPage;
  newTblObject->physicalPageNumber = physicalPage;
}

void printInfo(int numAddresses, int pageFaults, int tlbHits){
  printf("num of translated address = %d\n",numAddresses);
  printf("Page Faults = %d\n",pageFaults );
  float pagePercent=(float)pageFaults/numAddresses*100;
  printf("Page Fault Rate = %f%%\n",pagePercent);
  printf("TLB Hits = %d\n",tlbHits);
  float tlbPercent=(float)tlbHits/numAddresses*100;
  printf("TLB Hit Rate = %f%%\n",tlbPercent);
}

int main(int argc, char *argv[]){

  FILE *fp;
  int numAddresses = 0; //total num addresses
  char buffer[6830]; //contents in addresses.txt

  int logicalAddresses; //the int value being read from addresses.txt

  int pageFaults=0; //page fault rate = % of address references -> page faults
  int tlbHits=0; //TLB hit rate = % of addreses references that were resolved in the TLB


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

  for(int i = 0; i<PAGES;i++){
    pageTable[i] = -1;
  }

  //opening BACKING_STORE.bin
  char* binFile = "BACKING_STORE.bin";
  int backingStore = open(binFile,O_RDONLY);
  if (backingStore == -1){
    fprintf((stderr), "Backing file didn't open :( \n" );
    exit(1);
  }

  backingChars = mmap(0,MEMORY_SIZE,PROT_READ,MAP_PRIVATE,backingStore,0);

  //prints the buffer/addresses
  while(fgets(buffer,BUFFER_SIZE,fp)!=NULL){
    numAddresses++;
    int logicalAddress = atoi(buffer);
    int offset = logicalAddress % 256;
    int logicalPage = logicalAddress/256;
    int physicalPage=searchTlbTable(logicalPage);

    unsigned int freePage = 0;
    if(physicalPage !=-1){
      tlbHits++;
    }
    else{
      physicalPage = pageTable[logicalPage];
      if(physicalPage == -1){
        pageFaults++;
        physicalPage = freePage;
        freePage++;
        //copy page from backing store file into physical mem
        memcpy(mainMem+physicalPage*PAGE_SIZE, backingChars+logicalPage*PAGE_SIZE, PAGE_SIZE);
        pageTable[logicalPage] = physicalPage;
      }

      addToTlb(logicalPage, physicalPage);
    }

    int physicalAdd = (physicalPage << OFFSET) | offset;
    signed char value = mainMem[physicalPage*PAGE_SIZE+offset];
    printf("Logical address: %d Physical Address: %d Value:%d\n",logicalAddress,physicalAdd,value);
  }

  fclose(fp);     //close file
  printInfo(numAddresses,pageFaults,tlbHits);

  return 0;
}
