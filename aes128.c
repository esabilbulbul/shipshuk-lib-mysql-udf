
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "aes128.h"

#define AES_BLOCK_SIZE 16
#define AES_KEY_SIZE   16
#define AES_ROUNDS     10

/* =========================================================
   RANDOM IV GENERATION
   Standard C only: no OS-specific dependencies.
   ========================================================= */

static void generate_random(unsigned char *buffer, size_t length)
{
    static int initialized = 0;

    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }

    for (size_t i = 0; i < length; i++) {
        buffer[i] = (unsigned char)(
            (unsigned long)rand() * 256UL /
            ((unsigned long)RAND_MAX + 1UL)
        );
    }
}


/* =========================================================
   AES MATHEMATICAL OPERATIONS
   ========================================================= */

static unsigned char gf_mul(unsigned char a, unsigned char b)
{
    unsigned char result = 0;

    for (int i = 0; i < 8; i++) {

        if (b & 1)
            result ^= a;

        unsigned char high = a & 0x80;

        a <<= 1;

        if (high)
            a ^= 0x1B;

        b >>= 1;
    }

    return result;
}

static unsigned char gf_pow(unsigned char a, int power)
{
    unsigned char result = 1;

    while (power) {

        if (power & 1)
            result = gf_mul(result, a);

        a = gf_mul(a, a);
        power >>= 1;
    }

    return result;
}

static unsigned char rotl8(unsigned char x, int n)
{
    return (unsigned char)((x << n) | (x >> (8 - n)));
}

static unsigned char aes_sbox(unsigned char x)
{
    unsigned char y = x ? gf_pow(x, 254) : 0;

    return y ^ rotl8(y, 1) ^
           rotl8(y, 2) ^ rotl8(y, 3) ^
           rotl8(y, 4) ^ 0x63;
}

static unsigned char aes_inv_sbox(unsigned char x)
{
    unsigned char y =
        rotl8(x, 1) ^ rotl8(x, 3) ^
        rotl8(x, 6) ^ 0x05;

    return y ? gf_pow(y, 254) : 0;
}


/* =========================================================
   AES-128 KEY EXPANSION
   ========================================================= */

static void aes_key_expand(
    const unsigned char *key,
    unsigned char *round_key)
{
    static const unsigned char rcon[10] = {
        0x01, 0x02, 0x04, 0x08, 0x10,
        0x20, 0x40, 0x80, 0x1B, 0x36
    };

    memcpy(round_key, key, AES_KEY_SIZE);

    unsigned char temp[4];

    for (int i = 4; i < 44; i++) {

        memcpy(temp, round_key + (i - 1) * 4, 4);

        if (i % 4 == 0) {

            unsigned char t = temp[0];

            temp[0] = aes_sbox(temp[1]) ^ rcon[i / 4 - 1];
            temp[1] = aes_sbox(temp[2]);
            temp[2] = aes_sbox(temp[3]);
            temp[3] = aes_sbox(t);
        }

        for (int j = 0; j < 4; j++) {
            round_key[i * 4 + j] =
                round_key[(i - 4) * 4 + j] ^ temp[j];
        }
    }
}


/* =========================================================
   AES ROUND OPERATIONS
   ========================================================= */

static void add_round_key(
    unsigned char *state,
    const unsigned char *key)
{
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        state[i] ^= key[i];
}

static void sub_bytes(unsigned char *state)
{
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        state[i] = aes_sbox(state[i]);
}

static void inv_sub_bytes(unsigned char *state)
{
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        state[i] = aes_inv_sbox(state[i]);
}

static void shift_rows(unsigned char *s)
{
    unsigned char t;

    t = s[1];
    s[1] = s[5];
    s[5] = s[9];
    s[9] = s[13];
    s[13] = t;

    t = s[2];
    s[2] = s[10];
    s[10] = t;

    t = s[6];
    s[6] = s[14];
    s[14] = t;

    t = s[15];
    s[15] = s[11];
    s[11] = s[7];
    s[7] = s[3];
    s[3] = t;
}

static void inv_shift_rows(unsigned char *s)
{
    unsigned char t;

    t = s[13];
    s[13] = s[9];
    s[9] = s[5];
    s[5] = s[1];
    s[1] = t;

    t = s[2];
    s[2] = s[10];
    s[10] = t;

    t = s[6];
    s[6] = s[14];
    s[14] = t;

    t = s[3];
    s[3] = s[7];
    s[7] = s[11];
    s[11] = s[15];
    s[15] = t;
}

static void mix_columns(unsigned char *s)
{
    for (int i = 0; i < 4; i++) {

        unsigned char *c = s + i * 4;

        unsigned char a = c[0];
        unsigned char b = c[1];
        unsigned char d = c[2];
        unsigned char e = c[3];

        c[0] = gf_mul(a, 2) ^ gf_mul(b, 3) ^ d ^ e;
        c[1] = a ^ gf_mul(b, 2) ^ gf_mul(d, 3) ^ e;
        c[2] = a ^ b ^ gf_mul(d, 2) ^ gf_mul(e, 3);
        c[3] = gf_mul(a, 3) ^ b ^ d ^ gf_mul(e, 2);
    }
}

static void inv_mix_columns(unsigned char *s)
{
    for (int i = 0; i < 4; i++) {

        unsigned char *c = s + i * 4;

        unsigned char a = c[0];
        unsigned char b = c[1];
        unsigned char d = c[2];
        unsigned char e = c[3];

        c[0] = gf_mul(a, 14) ^ gf_mul(b, 11) ^
               gf_mul(d, 13) ^ gf_mul(e, 9);

        c[1] = gf_mul(a, 9) ^ gf_mul(b, 14) ^
               gf_mul(d, 11) ^ gf_mul(e, 13);

        c[2] = gf_mul(a, 13) ^ gf_mul(b, 9) ^
               gf_mul(d, 14) ^ gf_mul(e, 11);

        c[3] = gf_mul(a, 11) ^ gf_mul(b, 13) ^
               gf_mul(d, 9) ^ gf_mul(e, 14);
    }
}


/* =========================================================
   AES-128 BLOCK ENCRYPTION
   ========================================================= */

static void aes_encrypt_block(
    unsigned char *block,
    const unsigned char *round_key)
{
    add_round_key(block, round_key);

    for (int round = 1; round < AES_ROUNDS; round++) {

        sub_bytes(block);
        shift_rows(block);
        mix_columns(block);

        add_round_key(block, round_key + round * 16);
    }

    sub_bytes(block);
    shift_rows(block);

    add_round_key(block, round_key + 160);
}


/* =========================================================
   AES-128 BLOCK DECRYPTION
   ========================================================= */

static void aes_decrypt_block(
    unsigned char *block,
    const unsigned char *round_key)
{
    add_round_key(block, round_key + 160);

    for (int round = 9; round > 0; round--) {

        inv_shift_rows(block);
        inv_sub_bytes(block);

        add_round_key(block, round_key + round * 16);

        inv_mix_columns(block);
    }

    inv_shift_rows(block);
    inv_sub_bytes(block);

    add_round_key(block, round_key);
}


/* =========================================================
   BASE64 ENCODING
   ========================================================= */

static const char b64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static char *base64_encode(
    const unsigned char *data,
    size_t length,
    size_t *output_length)
{
    if (length > SIZE_MAX - 2)
        return NULL;

    size_t groups = (length + 2) / 3;

    if (groups > (SIZE_MAX - 1) / 4)
        return NULL;

    size_t out_len = groups * 4;

    char *out = malloc(out_len + 1);

    if (!out)
        return NULL;

    size_t i = 0;
    size_t j = 0;

    while (i < length) {

        size_t remaining = length - i;

        uint32_t a = data[i++];
        uint32_t b = remaining > 1 ? data[i++] : 0;
        uint32_t c = remaining > 2 ? data[i++] : 0;

        uint32_t v = (a << 16) | (b << 8) | c;

        out[j++] = b64_table[(v >> 18) & 63];
        out[j++] = b64_table[(v >> 12) & 63];

        out[j++] = remaining > 1
            ? b64_table[(v >> 6) & 63] : '=';

        out[j++] = remaining > 2
            ? b64_table[v & 63] : '=';
    }

    out[j] = '\0';

    if (output_length)
        *output_length = j;

    return out;
}


/* =========================================================
   BASE64 DECODING
   ========================================================= */

static int b64_value(char c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;

    return -1;
}

static unsigned char *base64_decode(
    const char *input,
    size_t *output_length)
{
    size_t len = strlen(input);

    if (!len || len % 4 != 0)
        return NULL;

    if (len > SIZE_MAX / 3 * 4)
        return NULL;

    size_t capacity = (len / 4) * 3;

    unsigned char *out = malloc(capacity);

    if (!out)
        return NULL;

    size_t j = 0;

    for (size_t i = 0; i < len; i += 4) {

        int last = (i + 4 == len);

        int a = b64_value(input[i]);
        int b = b64_value(input[i + 1]);

        int c = input[i + 2] == '='
            ? 0 : b64_value(input[i + 2]);

        int d = input[i + 3] == '='
            ? 0 : b64_value(input[i + 3]);

        if (a < 0 || b < 0 || c < 0 || d < 0 ||
            (!last && (input[i + 2] == '=' ||
                       input[i + 3] == '=')) ||
            (input[i + 2] == '=' && input[i + 3] != '=') ||
            (input[i + 2] == '=' && (b & 15)) ||
            (input[i + 3] == '=' && input[i + 2] != '=' && (c & 3))) {

            free(out);
            return NULL;
        }

        uint32_t v =
            ((uint32_t)a << 18) |
            ((uint32_t)b << 12) |
            ((uint32_t)c << 6) |
            (uint32_t)d;

        out[j++] = (v >> 16) & 255;

        if (input[i + 2] != '=')
            out[j++] = (v >> 8) & 255;

        if (input[i + 3] != '=')
            out[j++] = v & 255;
    }

    *output_length = j;

    return out;
}


/* =========================================================
   AES-128-CBC ENCRYPTION
   Output: Base64(IV + ciphertext)
   ========================================================= */

AESResult aes_encrypt(
    const unsigned char *plaintext,
    const unsigned char *key)
{
    AESResult result = { NULL, 0 };

    if (!plaintext || !key)
        return result;

    size_t input_len = strlen((const char *)plaintext);

    if (input_len > SIZE_MAX - AES_BLOCK_SIZE)
        return result;

    size_t padded_len =
        ((input_len / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;

    unsigned char *buffer = malloc(padded_len);

    if (!buffer)
        return result;

    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char round_key[176];

    generate_random(iv, AES_BLOCK_SIZE);

    aes_key_expand(key, round_key);

    memcpy(buffer, plaintext, input_len);

    unsigned char padding =
        (unsigned char)(padded_len - input_len);

    memset(buffer + input_len, padding, padding);

    unsigned char previous[AES_BLOCK_SIZE];

    memcpy(previous, iv, AES_BLOCK_SIZE);

    for (size_t i = 0; i < padded_len; i += AES_BLOCK_SIZE) {

        for (int j = 0; j < AES_BLOCK_SIZE; j++)
            buffer[i + j] ^= previous[j];

        aes_encrypt_block(buffer + i, round_key);

        memcpy(previous, buffer + i, AES_BLOCK_SIZE);
    }

    unsigned char *combined =
        malloc(AES_BLOCK_SIZE + padded_len);

    if (!combined) {
        free(buffer);
        return result;
    }

    memcpy(combined, iv, AES_BLOCK_SIZE);

    memcpy(
        combined + AES_BLOCK_SIZE,
        buffer,
        padded_len
    );

    result.data = base64_encode(
        combined,
        AES_BLOCK_SIZE + padded_len,
        &result.length
    );

    free(combined);
    free(buffer);

    return result;
}


/* =========================================================
   AES-128-CBC DECRYPTION
   Input: Base64(IV + ciphertext)
   ========================================================= */

AESResult aes_decrypt(
    const char *encrypted,
    const unsigned char *key)
{
    AESResult result = { NULL, 0 };

    if (!encrypted || !key)
        return result;

    size_t decoded_len = 0;

    unsigned char *decoded =
        base64_decode(encrypted, &decoded_len);

    if (!decoded)
        return result;

    if (decoded_len <= AES_BLOCK_SIZE ||
        (decoded_len - AES_BLOCK_SIZE) % AES_BLOCK_SIZE != 0) {

        free(decoded);
        return result;
    }

    size_t ciphertext_len = decoded_len - AES_BLOCK_SIZE;

    unsigned char round_key[176];

    aes_key_expand(key, round_key);

    unsigned char *plaintext =
        malloc(ciphertext_len + 1);

    if (!plaintext) {
        free(decoded);
        return result;
    }

    memcpy(
        plaintext,
        decoded + AES_BLOCK_SIZE,
        ciphertext_len
    );

    unsigned char previous[AES_BLOCK_SIZE];
    unsigned char current[AES_BLOCK_SIZE];

    memcpy(previous, decoded, AES_BLOCK_SIZE);

    for (size_t i = 0; i < ciphertext_len; i += AES_BLOCK_SIZE) {

        memcpy(current, plaintext + i, AES_BLOCK_SIZE);

        aes_decrypt_block(plaintext + i, round_key);

        for (int j = 0; j < AES_BLOCK_SIZE; j++)
            plaintext[i + j] ^= previous[j];

        memcpy(previous, current, AES_BLOCK_SIZE);
    }

    /* Validate PKCS#7 padding */

    unsigned char padding = plaintext[ciphertext_len - 1];

    if (padding == 0 || padding > AES_BLOCK_SIZE) {
        free(decoded);
        free(plaintext);
        return result;
    }

    unsigned char bad = 0;

    for (int i = 0; i < padding; i++) {
        bad |= plaintext[ciphertext_len - 1 - i] ^ padding;
    }

    if (bad) {
        free(decoded);
        free(plaintext);
        return result;
    }

    size_t plain_len = ciphertext_len - padding;

    plaintext[plain_len] = '\0';

    free(decoded);

    result.data = (char *)plaintext;
    result.length = plain_len;

    return result;
}