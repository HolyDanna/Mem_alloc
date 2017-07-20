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
 * Created on 19 novembre 2015, 14:32
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
    #include <time.h>
    #include <stdlib.h>

    void*Mem_Alloc(int size);    
    int Mem_Free(void *ptr);
    int Mem_IsValid(void *ptr);
    int Mem_GetSize(void *ptr);
    /*int size_region, addr_region;*/    
    


#ifdef __cplusplus
}
#endif

#endif /* MEM_ALLOC_H */

