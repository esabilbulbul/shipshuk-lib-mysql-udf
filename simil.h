/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   simil.h
 * Author: Administrator
 *
 * Created on February 26, 2017, 11:28 AM
 */

#include "libexport.h"

#ifndef SIMIL_H
#define SIMIL_H

    #ifdef __cplusplus
    extern "C" {
    #endif


        //DLL_EXPORT int simil (const char *a, const char *b);

		// Define DLL_EXPORT based on platform
        #ifdef _WIN32
            #define DLL_EXPORT __declspec(dllexport)
        #else
            #define DLL_EXPORT __attribute__((visibility("default")))
        #endif

        // Declare the simil function with DLL_EXPORT for proper export
        DLL_EXPORT int simil(const char *a, const char *b);

    #ifdef __cplusplus
    }
    #endif

#endif /* SIMIL_H */

