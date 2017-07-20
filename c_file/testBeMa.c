/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: alexis
 * Created under licence GNU GPL since 22 november 2015, 21:23
 * Created on 18 novembre 2015, 21:55
 */

#include "mem.h"
#include "../lib/Mem_Free.h"

/*
 * 
 */
#define SIZE_MAX_ALLOCATE pow(2,28)
#define SIZE_REDUCT pow(2,10)
#define SIZE_STRUCT 40
#define PSIZE 4096
#define AVERAGE_NUMBER_ALLOCATION_T_SIZE_ALLOCATION 3000 /*(SIZE_MAX_ALLOCATE/((SIZE_MAX_ALLOCATE/SIZE_REDUCT)/2))*/

void test_mem_free(){
    
    int r,t,i,j,k;
    void *addr[20];
    int size[20];
    
    printf("\n-----------------------------Test START: test_size_allocation()-----------------------------\n");
    
    srand(time(NULL));
    
    printf("\n--------\nThis test is about allocating 20 blocks of pseudo-random size, allocating values in these chunks of memory. Then freeing half of these chunks, see if the unfred chunks are still untouched. Finally free the last chunks, and verify that we only have one big chunk \n--------\n");
    
    for (i=0;i<20;i++){
        r=rand()%5+1;
        t=rand()%2;
        if (t==0){
            addr[i]=Mem_Alloc(r*sizeof(long int));
            size[i]=8*r;
            printf("LONG INT : ");
            printf("Memory allocation valid %d : ",Mem_IsValid(addr[i]));
            for (j=0;j<r;j++){
                *((long int*)((long int)addr[i] + j*sizeof(long int)))=j;
            }
            printf("j=%d\n",j);
        } else if (t==1) {
            addr[i]=Mem_Alloc(r*sizeof(int));
            size[i]=4*r;
            printf("INT : ");
            printf("Memory allocation valid %d : ",Mem_IsValid(addr[i]));
            for (j=0;j<r;j++){
                *((int *)((long int)addr[i] + j*sizeof(int)))=j;
            }
            printf("j=%d\n",j);
        }
        
    }
    
    for (i=0;i<20;i++){
        printf("Block n°%d : size = %d : addr = %ld\n",i,Mem_GetSize(addr[i]),(long int)addr[i]);
    }
    
    printf("\n--------\nThe allocation is done. Now freeing half of the blocks, and then checking the 'size' of all blocks \n--------\n");
    for (i=0;i<10;i++){
        Mem_Free(addr[2*i]);
    }
    
    for (i=0;i<20;i++){
        printf("Block n°%d : size = %d : addr = %ld\n",i,Mem_GetSize(addr[i]),(long int)addr[i]);
    }
    
    for (i=0;i<5;i++){
        Mem_Free(addr[4*i+1]);
    }
    
    printf("\n--------\nFreeing half of the still occupied blocks. \n--------\n");
    for (i=0;i<20;i++){
        printf("Block n°%d : size = %d : addr = %ld\n",i,Mem_GetSize(addr[i]),(long int)addr[i]);
    }
    
    for (i=0;i<5;i++){
        k=size[4*i]+size[4*i+1]+size[4*i+2]+80;
        addr[i*4]=Mem_Alloc(k);
        size[i*4]=k;
    }
    
    printf("\n--------\nAllocating the space left after the free\n--------\n");
    for (i=0;i<20;i++){
        printf("Block n°%d : size = %d : addr = %ld\n",i,Mem_GetSize(addr[i]),(long int)addr[i]);
    }
    
    printf("\n--------\nFreeing everything\n--------\n");
    
    for (i=0;i<20;i++){
        Mem_Free(addr[i]);
        printf("Block n°%d : size = %d : addr = %ld\n",i,Mem_GetSize(addr[i]),(long int)addr[i]);
    }
   
}

void test_size_allocation(){
    
    long int size,size_free;
    int r,prompt,rprompt,i;
    void *addr[AVERAGE_NUMBER_ALLOCATION_T_SIZE_ALLOCATION];
    
    printf("\n-----------------------------Test START: test_size_allocation()-----------------------------\n");
    
    srand(time(NULL));
    prompt=0;
    rprompt=0;
    size_free=SIZE_MAX_ALLOCATE;
    size_free= (size_free%PSIZE ? size_free : ( ((int)size_free/PSIZE +1)*PSIZE));
    size_free=size_free-SIZE_STRUCT;
    
    printf("\n--------\nfirst 'size_free' correspond to 'SIZE_MAX_ALLOCATE' cut in page size:%ld\n--------\n",size_free);

    while(size_free>0){
        
        r=rand();
        size=r;
        
        size=(int)(((((double)size)/RAND_MAX))*(SIZE_MAX_ALLOCATE/SIZE_REDUCT));
        
        
        if(size<1){
            size=1;
        }
        
        addr[rprompt]=Mem_Alloc(size);
        printf("\n--------\nADDR:%ld\n--------\n",(long int) addr[rprompt]);
        
        if((long int)addr[rprompt]>1){
            printf("\n--------\nsize allocated:%ld\n--------\n",size);
            size_free=size_free-size-SIZE_STRUCT;
            printf("\n--------\nsize_free:%ld\n--------\n",size_free);
            rprompt++;
        }
        
        if(size!=Mem_GetSize(addr[rprompt]) && addr[rprompt]!=NULL){
            printf("\n--------\nErreur d'allocation:\nsize:%ld\naddr:%ld\nprompt:%d\nrprompt:%d\n--------\n",size,(long int)addr[rprompt],prompt,rprompt);
            exit(0);
            
        }
        
        prompt++;
        
        if(size_free<1){
            printf("\n--------\nSize 'size_free' attend: -40\n--------\n");
             
            printf("\n--------\ntest_size_allocation terminated!\nprompt:%d\nrprompt:%d\nsize_free:%ld\n--------\n",prompt,rprompt,size_free);
             
        }
               
    }
    printf("\n--------\nNUMBER_ALLOCATION:%d\n--------\n",rprompt);
    printf("\n--------\nNUMBER_ALLOCATION_FAILED:%d\n--------\n",prompt-rprompt);
    printf("\n-----------------------------Test CONTINUE: test_size_allocation()-----------------------------\n");
    rprompt-=1;
    while(rprompt>-1){
        i=Mem_Free(addr[rprompt]);
        /*printf("\n--------\nReturn Mem_Free:%d\n--------\n",i);*/
        rprompt--;
        if (i==-1){
            printf("\n-----------------------------Test ERROR: test_size_allocation()-----------------------------\n");
            printf("\n-----------------------------Test ERROR: during EXEC of Mem_Free()--------------------------\n");
            exit(0);
        }
    }
    
    printf("\n--------\nSize 'size 1st block':%d\n--------\n",Mem_GetSize((void*)addr[0]));
    printf("\n--------\nSize 'size 2nd block':%d\n--------\n",Mem_GetSize((void*)addr[1]));
    printf("\n--------\nSize 'size 3rd block':%d\n--------\n",Mem_GetSize((void*)addr[2]));
            
    printf("\n-----------------------------Test END: test_size_allocation()-----------------------------\n");
   
}
void small_test_alloc(){
    int r,s,t,j;
    long int i[10];
    
    printf("\n-----------------------------Test START: small_test_alloc()-----------------------------\n");
    
    srand(time(NULL));
    r=rand();
    s=r;
    s=(int)(((double)s/RAND_MAX)*9 + 1);
      
    t=s;
    while(s>0){
        i[s]=(long int)Mem_Alloc(s*10);
        printf("\n--------\n%d e alloc: %ld\n--------\n",s,i[s]);
        printf("\n--------\nsize %d allocated block number: %d\n--------\n",Mem_GetSize((void *)i[s]),s);
        s--;
    }
    printf("\n-----------------------------Test CONTINUE: small_test_alloc()-----------------------------\n");
    while(s<t){
        s++;
        j=Mem_Free((void *)i[s]);
        printf("\n--------\nsize %d free block number: %d\n--------\n",Mem_GetSize((void*)i[s]),s);
        printf("\n--------\ni[s]:%ld\n--------\n",i[s]);
        printf("\n--------\nReturn Mem_Free:%d\n--------\n",j);
    }
    printf("\n-----------------------------Test END: small_test_alloc()-----------------------------\n");
}

int main(int argc, char** argv){
   test_size_allocation();
   small_test_alloc();
   test_mem_free();    
    return 0;
}
