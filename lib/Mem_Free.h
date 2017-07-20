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

#ifndef MEM_FREE_H
#define MEM_FREE_H

#ifdef __cplusplus
extern "C" {
#endif

    int Mem_IsValid(void *ptr);
    int Mem_GetSize(void *ptr);
    void *Mem_IsValid_Aux(void *ptr);
    long int *get_add_region();
    long int get_size_region();


#ifdef __cplusplus
}
#endif

#endif /* MEM_FREE_H */

