/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mysql_fun.h
 * Author: Administrator
 *
 * Created on February 24, 2017, 11:05 AM
 */


#ifndef SHIPSHUK_LIB_H
#define SHIPSHUK_LIB_H

typedef unsigned longlong;
typedef char my_bool;

    #ifdef __GNUC__

        /* From Check:  http://check.svn.sourceforge.net/viewvc/check/trunk/src/check.h.in?revision=HEAD
           License: LGPL 2.1 or later */
        #ifdef __GNUC_MINOR__
        #define GCC_VERSION_AT_LEAST(major, minor) \
                        ((__GNUC__ > (major)) || \
                         (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))
        #else
        #define GCC_VERSION_AT_LEAST(major, minor) 0
        #endif /* __GNUC_MINOR__ */

        #if GCC_VERSION_AT_LEAST(2, 95)
        #define ATTRIBUTE_UNUSED __attribute__ ((unused))
        #else
        #define ATTRIBUTE_UNUSED 
        #endif
        #else
        #define ATTRIBUTE_UNUSED 

    #endif



    #ifdef __cplusplus
    extern "C" {
    #endif

        DLL_EXPORT my_bool SIMIL_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
        
        DLL_EXPORT void SIMIL_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED);
/*
        DLL_EXPORT char* SIMIL( UDF_INIT       *initid, 
                                UDF_ARGS        *args,
                                char            *result,
                                unsigned long   *res_length,
                                char            *null_value, 
                                char            *error);
*/
        DLL_EXPORT longlong SIMIL(  UDF_INIT *initid, UDF_ARGS *args, char     *is_null, char     *error);
        
        DLL_EXPORT my_bool DISTANCE_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
        DLL_EXPORT void DISTANCE_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED);
        
        DLL_EXPORT double DISTANCE(  UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
        
        
        
        DLL_EXPORT my_bool shipshuk_test_init(UDF_INIT *initid, UDF_ARGS *args, char *message);

        DLL_EXPORT void shipshuk_test_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED);

        DLL_EXPORT char *shipshuk_test( UDF_INIT       *initid, 
                                        UDF_ARGS        *args,
                                        char            *result, 
                                        unsigned long   *res_length,
                                        char            *null_value, 
                                        char            *error);

        DLL_EXPORT int win_test();

        
        DLL_EXPORT my_bool ISIMIL_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
        
        DLL_EXPORT void ISIMIL_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED);
        
        DLL_EXPORT unsigned long ISIMIL(  UDF_INIT *initid, UDF_ARGS *args, char     *is_null, char     *error);
        
    #ifdef __cplusplus
    }
    #endif

#endif /* SHIPSHUK_LIB_H */

