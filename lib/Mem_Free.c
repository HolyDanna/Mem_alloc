/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 *
 * Created under licence GNU GPL since 22 november 2015, 21:23 
 *
 */
#include"Mem_Alloc.h"
#include"Mem_Free.h"



int Mem_Free(void *ptr){
    long int *ad_region,size_region,addr;
    res_mem_t res_free,res_next,res_previous;
    ad_region=get_add_region();
    size_region=get_size_region();
    
    if (((long int)ad_region < (long int) ptr)&&((long int)ad_region)+size_region > (long int)ptr) {
        
        addr= (long int)Mem_IsValid_Aux(ptr);
        /*printf("PTR:%ld\n",(long int)ptr);
        printf("ADDR:%ld\n",addr);*/
        
        
        
        if (addr> 0 /*CHANGE*/) {
           
            res_free=(void*)addr;
            /*first step is freeing the current block*/
            res_free->state_free=1;


            res_next=res_free->addr_next;

            res_previous=res_free->addr_previous;


            /*next, we see if the previous block is free, and if so, we fuse it with the recently fred block*/
            if (res_free->addr_previous!=NULL){
                if(res_previous->state_free){

                    res_previous->size_block=res_previous->size_block+res_free->size_block+5*sizeof(res_mem_t);
                    res_previous->addr_next=res_free->addr_next;
                    res_next->addr_previous=res_previous->addr_current;

                    res_free->addr_current=NULL;
                    res_free->addr_next=NULL;
                    res_free->addr_previous=NULL;
                    res_free->state_free=1;
                    res_free=res_previous;
                }
            }

            /*then we see if the following block is free, and if so, we fuse it with the recently fred block*/
            if (res_free->addr_next!=NULL){
                if(res_next->state_free){
                    /*Mem_Free((void*)res_next);*/

                    res_free->size_block=res_free->size_block+res_next->size_block+5*sizeof(res_mem_t);

                    res_free->addr_next=res_next->addr_next;
                    res_next->addr_current=NULL;
                    res_next->addr_next=NULL;
                    res_next->addr_previous=NULL;
                    res_next->state_free=1;

                    /*ADDDD START*/

                    if (res_free->addr_next!=NULL) {
                        /*res_next is now the elements after the original res_next*/
                        res_next=res_free->addr_next;
                        res_next->addr_previous=res_free->addr_current;
                    }

                    /*ADDDD END*/


                    /*res_previous->addr_previous=res_free->addr_current;*/
                    /*printf("TEST54\n");*/
                }
            }
        }
           
        return 0;
    
    }
    return -1;
}
