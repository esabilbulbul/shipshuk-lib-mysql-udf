/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   distance.h
 * Author: Administrator
 *
 * Created on February 15, 2019, 4:30 PM
 */
#include "libexport.h"

#ifndef DISTANCE_H
#define DISTANCE_H

    #ifdef __cplusplus
    extern "C" {
    #endif

    DLL_EXPORT double distance(double lat1, double lon1, double lat2, double lon2, char unit);


    #ifdef __cplusplus
    }
    #endif

#endif /* DISTANCE_H */

