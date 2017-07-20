/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mem_Alloc.h
 * Author: alexis
 *
 * Created under licence GNU GPL since 22 november 2015, 21:23 
 *
 * Created on 20 novembre 2015, 16:46
 */

#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <sys/mman.h>
    #include <sys/types.h>
    #include <unistd.h>
 
typedef struct{
    void *addr_previous;
    void *addr_current;
    long int size_block;
    long int state_free;
    void *addr_next;
} *res_mem_t;


#ifdef __cplusplus
}
#endif

#endif /* MEM_ALLOC_H */

