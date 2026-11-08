#ifndef AES128_H
#define AES128_H

#include <stddef.h>
#include "libexport.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        char *data;
        size_t length;
    } AESResult;

    DLL_EXPORT AESResult aes_encrypt(
        const unsigned char *plaintext,
        const unsigned char *key
    );

    DLL_EXPORT AESResult aes_decrypt(
        const char *encrypted,
        const unsigned char *key
    );

#ifdef __cplusplus
}
#endif

#endif