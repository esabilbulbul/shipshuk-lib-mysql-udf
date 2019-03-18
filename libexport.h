/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   libexport.h
 * Author: Administrator
 *
 * Created on February 24, 2017, 10:55 AM
 */

#ifndef LIBEXPORT_H
#define LIBEXPORT_H

    #ifdef __cplusplus
    extern "C" {
    #endif


        #ifdef SAMPLE_DLL
            #define DLL_EXPORT __declspec(dllexport)
        #else
            #define DLL_EXPORT __declspec(dllimport)
        #endif


    #ifdef __cplusplus
    }
    #endif

#endif /* LIBEXPORT_H */


