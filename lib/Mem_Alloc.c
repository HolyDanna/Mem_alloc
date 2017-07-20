/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 *
 *Created under licence GNU GPL since 22 november 2015, 21:23
 *
 */
#include "Mem_Alloc.h"


#define MAP_ANONYMOUS 0x20
#define MIN_SIZE_REGION pow(2,27)

long int size_region, *addr_region;
int JustDoIt=1;


int Mem_Init(int sizeOfRegion){
        
        int psize;
        double approxim;
        res_mem_t res0;
        size_region=sizeOfRegion;
        psize=sysconf(_SC_PAGESIZE);/*getpagesize();*/ /*POSIX since 2001 removed 'getpagesize();', today, its used is depreceated*/
        /*printf("test:%lf\n",floor(d));*/
        approxim=size_region/psize;
        
        size_region= (size_region%psize ? size_region : ( ((int)approxim +1)*psize));
        addr_region=mmap(NULL, size_region,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,-1,0); 
        
        /*printf("addr_region %ld \n",(long int) addr_region);*/
        res0=(res_mem_t) addr_region;
        res0->addr_current=addr_region;
        res0->size_block=size_region-5*sizeof(res_mem_t);
        res0->addr_previous=NULL;
        res0->addr_next=NULL;
        res0->state_free=1;
        /*printf("size-Region: %ld\n",size_region);*/
        return  0;
}

long int Mem_IsValid_Aux(void *ptr){
    void* i; 
    unsigned long int j, k;
    res_mem_t res_test;
    i=addr_region;
    k=(long int) ptr;
    while (i!=NULL){
        res_test=i;
        j=(long int) i;
        if (j+(long int)res_test->size_block+5*sizeof(res_test)>k && k>j-1+5*sizeof(res_test)){
            if (!res_test->state_free)
                return j;
            else
                return -j;
        }
        i=res_test->addr_next;
    }
    return -1;
}
int Mem_IsValid(void *ptr){
    if (Mem_IsValid_Aux(ptr)>0){
        return 1;
    } else {
        return -1;
    }
    
    /*void* i=addr_region; 
    res_mem_t res_test;
    while (i!=NULL){
        res_test=i;
        if (i+res_test.size_block+sizeof(res_mem_t)>ptr && ptr+1 > i+sizeof(res_mem_t)){
            return ((1-res_test.state_free)*2 -1);
        }
        i=res_test.addr_next;
    }
    return -1;*/
}

int Mem_GetSize(void *ptr){
    void *addr=(void*)Mem_IsValid_Aux(ptr);
    if ((long int) addr>0){
        res_mem_t res_test=addr;
        return res_test->size_block;
    /*} else if ((long int) addr != -1) {
        res_mem_t res_test=(void*)(- (long int)addr);
        return res_test->size_block;*/
    } else {
        return -1;
    }
}

void *Mem_Alloc(int size){/*Mem_Alloc isn't able to protect its structures because 'malloc' use a dynamic system where malloc call the function mmap a lot of time.*/
    
    void* i, *min_addr; 
    long int min_size,test_size;
    res_mem_t res_test,new_seg;
    
    if(JustDoIt){
        int size2 = (size < MIN_SIZE_REGION ? MIN_SIZE_REGION : size);
        Mem_Init(size2*2); /*Some security, in case the caller wants some big-sized chunks of memory. May cause failures if the chunks are too big (bigger than 512MB)*/
        JustDoIt=1-JustDoIt;
    }
    
    i=addr_region;
    min_size=-1;
    min_addr=(void*)-1;
    
    if (size == 0 || size>size_region){
        return NULL;
    }
    
    while (i!=NULL){
        res_test=i;
        
        if (res_test->state_free && res_test->size_block == size) {
            res_test->state_free=0;
           /* printf("FULL\n");
            printf("addr_region %ld \n",(long int) addr_region);
            printf("size-Region: %ld\n",size_region);*/
            return (void*) (((long int)i)+5*sizeof(res_mem_t));
        } /*this case is used if there is a chunk of memory with the exact desired size, in the free memory*/
        
        if (res_test->state_free && res_test->size_block > size) {
            test_size=res_test->size_block -size -10*sizeof(res_mem_t);
            
            if (test_size > -1) {
                
                if (min_size<0){ /*min_size is the smallest chunk of memory left after the space that was given*/
                    min_size=test_size;
                    min_addr=i;
                } else if (test_size < min_size) {
                    min_size=test_size;
                    min_addr=i;
                }
            }
        }
        i=res_test->addr_next;    
    }
    
    if (min_size>-1) {
        res_test=min_addr; /*res_test is the chunk I'll be givin' to the user. new_seg is the free chunk left after*/
        new_seg=(void*)((long int)min_addr+5*sizeof(res_mem_t)+size);
        
       /* printf("%d : %ld : %ld\n",size,res_test->size_block,min_size+40);*/
        
        new_seg->addr_next=res_test->addr_next;
        new_seg->addr_current=(void*)((long int)min_addr+5*sizeof(res_mem_t)+size);
        new_seg->size_block=(long int)res_test->size_block-5*sizeof(res_mem_t)-size;
        new_seg->state_free=1;
        new_seg->addr_previous=res_test;
        
        res_test->addr_next=new_seg;
        res_test->size_block=size;
        res_test->state_free=0;
        
        min_addr=(void*)((long int)res_test->addr_current+5*sizeof(res_mem_t));
        
        return min_addr;
    }
    return NULL;
    
}

long int *get_add_region(){
    return(long int*) addr_region;
}

long int get_size_region(){
    return size_region;
}

