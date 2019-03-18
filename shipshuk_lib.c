/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#include <my_global.h>
#include <mysql.h>
//#include <m_ctype.h>

#define SAMPLE_DLL

#include "libexport.h"
#include "shipshuk_lib.h"
#include "simil.h"
#include "distance.h"
//#include "simil.h"

#ifdef __WIN__
    #define DLLEXP __declspec(dllexport)
    #else
    #define DLLEXP
#endif



#ifdef __cplusplus
extern "C" {
#endif



/******************************************************************************
**                      Function Declarations
******************************************************************************/

DLL_EXPORT my_bool shipshuk_test_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    //if (args->arg_count != 2)
    //{
        //Requires more argument for instance
        //x_strlcpy(message, "No arguments allowed (udf: lib_mysqludf_str_info)", MYSQL_ERRMSG_SIZE);
        return 1;
    //}

    initid->maybe_null = 0;
    initid->max_length = (4) - 1;//size of version
    initid->const_item = 1;
    return 0;
}

//void lib_mysqludf_str_info_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED)
DLL_EXPORT void shipshuk_test_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED)
{
    
}

DLL_EXPORT char *shipshuk_test(UDF_INIT       *initid, 
                            UDF_ARGS        *args,
                            char            *result,
                            unsigned long   *res_length,
                            char            *null_value, 
                            char            *error)
{
	strcpy(result, "esabil");
	*res_length = 7-1;
	return result;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//                              SIMIL Function
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
DLL_EXPORT my_bool SIMIL_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count != 3)
    {
        //Requires more argument for instance
        //x_strlcpy(message, "No arguments allowed (udf: lib_mysqludf_str_info)", MYSQL_ERRMSG_SIZE);
        return 1;
    }

    if (args->arg_type[0] != STRING_RESULT)
        return 1;
    
    if (args->arg_type[1] != STRING_RESULT)
        return 1;
    
    initid->maybe_null = 1;
    initid->max_length = 255+1;//20;//(4) - 1;
    //initid->const_item = 0;//1 if the result same for each calls
    return 0;
}

DLL_EXPORT void SIMIL_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED)
{
    free(initid->ptr); 
}

DLL_EXPORT longlong SIMIL(  UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
    //memcpy(result, args->args[0], args->lengths[0]);
    unsigned long iStrLen0 = args->lengths[0];
    unsigned long iStrLen1 = args->lengths[1];
    unsigned long iStrLen2 = 0;
    if (args->arg_count>2)
    {
        iStrLen2 = args->lengths[2];
    }

    int len0 = (int)iStrLen0;
    int len1 = (int)iStrLen1;
    int len2 = (int)iStrLen2;
    
    if (len0>=255)
        len0=255;
    
    char word0[255]="";
    char word1[255]="";
    
    memcpy(word0, args->args[0], len0);
    memcpy(word1, args->args[1], len1);
    
    int iRes = simil(word0, word1);
    
    //*res_length = len0 + len1;
    
    return iRes;
}

/*
DLL_EXPORT char* SIMIL( UDF_INIT       *initid, 
                        UDF_ARGS        *args,
                        char            *result,
                        unsigned long   *res_length,
                        char            *null_value, 
                        char            *error)
{
    //memcpy(result, args->args[0], args->lengths[0]);
    unsigned long iStrLen0 = args->lengths[0];
    unsigned long iStrLen1 = args->lengths[1];
    unsigned long iStrLen2 = 0;
    if (args->arg_count>2)
    {
        iStrLen2 = args->lengths[2];
    }
    //char sTest = 0x30 + iStrLen0;
    //int len = 6;
    int len0 = (int)iStrLen0;
    int len1 = (int)iStrLen1;
    int len2 = (int)iStrLen2;
    
    if (len0>=255)
        len0=255;
    
    //if (len1>=255)
    //    len1=strlen(args->lengths[1]);
    
    char word0[255]="";
    char word1[255]="";
    
    //memcpy(word0, args->args[1], len1);
    //memcpy(word1, args->args[1], len1);
    int index=0;
    result[index++] = 0x30 + args->arg_count;
    result[index++] = 0x30 + len0;
    result[index++] = 0x30 + len1;
    result[index++] = 0x30 + len2;

    memcpy(word0, args->args[0], len0);
    memcpy(word1, args->args[1], len1);
    
    memcpy(result     , word0, len0);
    memcpy(result+len0, word1, len1);
    //memcpy(result, word1, len1);
    
    int iRes = simil(word0, word1);
    
    *res_length = len0 + len1;
    
    return result;
}
*/

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//                              ISIMIL Function
//  This function is only for test purpose. To test the length of 
//  Parameters. No future use purpose
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
DLL_EXPORT my_bool ISIMIL_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    initid->maybe_null = 0;
    initid->max_length = sizeof(unsigned long);//20;//(4) - 1;
    initid->const_item = 1;
    return 0;
}

DLL_EXPORT void ISIMIL_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED)
{

}


DLL_EXPORT unsigned long ISIMIL(  UDF_INIT *initid, UDF_ARGS *args, char     *is_null, char     *error)
{
    
    return args->lengths[0];
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//                              CALC DISTANCE FUNCTION
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

DLL_EXPORT my_bool DISTANCE_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count != 5)
    {
        //Requires more argument for instance
        //x_strlcpy(message, "No arguments allowed (udf: lib_mysqludf_str_info)", MYSQL_ERRMSG_SIZE);
        return 1;
    }
    
    /*
    if (args->arg_type[0] != DECIMAL_RESULT)
        return 1;
    
    if (args->arg_type[1] != DECIMAL_RESULT)
        return 1;
    
    if (args->arg_type[2] != DECIMAL_RESULT)
        return 1;
    
    if (args->arg_type[3] != DECIMAL_RESULT)
        return 1;
    
    if (args->arg_type[4] != STRING_RESULT)
        return 1;
    */
    if (args->arg_type[4] != STRING_RESULT)
        return 1;

    
    initid->maybe_null = 1;
    initid->max_length = 255+1;//20;//(4) - 1;
    //initid->const_item = 0;//1 if the result same for each calls
    return 0;
}


DLL_EXPORT void DISTANCE_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED)
{
    free(initid->ptr);
}

DLL_EXPORT double DISTANCE(  UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
    
    char cParam1[500] = "";
    
    //memcpy(&lat1, args->args[0], sizeof(double));
    /*sprintf(sParams, "%d , %d , %d , %d , %d", args->lengths[0], args->lengths[1], args->lengths[2], args->lengths[3], args->lengths[4]);*/
    //memcpy(&lat1, args->args[0], sizeof(double));
    
    char cDistUnit;
    
    if (args->lengths[4]>0)
    {
        cDistUnit = args->args[4][0];
    }
    
    char cParam[100]="";
    int  iParam;
    double lat1 = 0;
    double lon1 = 0;
    double lat2 = 0;
    double lon2 = 0;
    for(int i=0; i<=3; i++)
    {
        memset(cParam, 0, 100);
        memcpy(cParam, args->args[i], args->lengths[i]);
        
        switch(i)
        {
            case 0:

                if (args->arg_type[i] == DECIMAL_RESULT)
                {
                    lat1 = atof(cParam);
                }
                else if (args->arg_type[i] == INT_RESULT)
                {
                    iParam = atoi(cParam);
                    lat1   = (double)iParam;
                }
                else
                {
                    lat1 = (double)0;
                }

                break;
                
                
            case 1:
                
                if (args->arg_type[i] == DECIMAL_RESULT)
                {
                    lon1 = atof(cParam);
                }
                else if (args->arg_type[i] == INT_RESULT)
                {
                    iParam = atoi(cParam);
                    lon1   = (double)iParam;
                }
                else
                {
                    lon1 = (double)0;
                }
                
                break;
            case 2:
                
                if (args->arg_type[i] == DECIMAL_RESULT)
                {
                    lat2 = atof(cParam);
                }
                else if (args->arg_type[i] == INT_RESULT)
                {
                    iParam = atoi(cParam);
                    lat2   = (double)iParam;
                }
                else
                {
                    lat2 = (double)0;
                }
                break;
            case 3:
                
                if (args->arg_type[i] == DECIMAL_RESULT)
                {
                    lon2 = atof(cParam);
                }
                else if (args->arg_type[i] == INT_RESULT)
                {
                    iParam = atoi(cParam);
                    lon2   = (double)iParam;
                }
                else
                {
                    lon2 = (double)0;
                }
                
                break;
        }
    }
    
    double dist = distance(lat1, lon1, lat2, lon2, cDistUnit);
    
    return dist;
    
    /*
    unsigned long iStrLen0 = args->lengths[0];
    unsigned long iStrLen1 = args->lengths[1];
    unsigned long iStrLen2 = 0;
    if (args->arg_count>2)
    {
        iStrLen2 = args->lengths[2];
    }

    int len0 = (int)iStrLen0;
    int len1 = (int)iStrLen1;
    int len2 = (int)iStrLen2;
    
    if (len0>=255)
        len0=255;
    
    char word0[255]="";
    char word1[255]="";
    
    memcpy(word0, args->args[0], len0);
    memcpy(word1, args->args[1], len1);
    */
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//                  CALC DISTANCE WIN TEST FUNCTION
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

my_bool win_calc_distance_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    return 0;
}

void win_calc_distance_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED)
{

}


DLL_EXPORT double win_calc_distance(double lat1, double lon1, double lat2, double lon2, char unit)
{
    double dRes =  distance(lat1, lon1, lat2, lon2, unit);
    return dRes;
}

//The following functions are test for from windows side (not mysql))

my_bool win_test_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    return 0;
}

void win_test_deinit(UDF_INIT *initid ATTRIBUTE_UNUSED)
{

}


DLL_EXPORT int win_test()
{
    char word1[] = "eshabil";
    char word2[] = "esabil";
    char word3[] = "sabil";
    char word4[] = "esxabil";
    char word5[] = "esha";
    /*
    int iRes1 = simil(word1, word2);
    int iRes2 = simil(word1, word3);
    int iRes3 = simil(word1, word4);
    int iRes4 = simil(word1, word5);
    */
    
    return 2;
}


#ifdef __cplusplus
}
#endif


