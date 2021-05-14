#ifndef __CHAD_STRING_H__
#define __CHAD_STRING_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

/* 'MAP' macro taken from https://github.com/swansontec/map-macro/blob/master/map.h */

#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0(EVAL0(EVAL0(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL4(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL(...)  EVAL4(EVAL4(EVAL4(__VA_ARGS__)))

#define MAP_END(...)
#define MAP_OUT
#define MAP_COMMA ,

#define MAP_GET_END2() 0, MAP_END
#define MAP_GET_END1(...) MAP_GET_END2
#define MAP_GET_END(...) MAP_GET_END1
#define MAP_NEXT0(test, next, ...) next MAP_OUT
#define MAP_NEXT1(test, next) MAP_NEXT0(test, next, 0)
#define MAP_NEXT(test, next)  MAP_NEXT1(MAP_GET_END test, next)

#define MAP0(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP1)(f, peek, __VA_ARGS__)
#define MAP1(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP0)(f, peek, __VA_ARGS__)

#define MAP_LIST_NEXT1(test, next) MAP_NEXT0(test, MAP_COMMA next, 0)
#define MAP_LIST_NEXT(test, next)  MAP_LIST_NEXT1(MAP_GET_END test, next)

#define MAP_LIST0(f, x, peek, ...) f(x) MAP_LIST_NEXT(peek, MAP_LIST1)(f, peek, __VA_ARGS__)
#define MAP_LIST1(f, x, peek, ...) f(x) MAP_LIST_NEXT(peek, MAP_LIST0)(f, peek, __VA_ARGS__)

/**
 * Applies the function macro `f` to each of the remaining parameters.
 */
#define MAP(f, ...) EVAL(MAP1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**
 * Applies the function macro `f` to each of the remaining parameters and
 * inserts commas between the results.
 */
#define MAP_LIST(f, ...) EVAL(MAP_LIST1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

////////////////////////////////////////////////////////////////////////////////////////////////////

#define PP_NARG(...) \
    PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) \
    PP_128TH_ARG(__VA_ARGS__)
#define PP_128TH_ARG(                                           \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,                    \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,           \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,           \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,           \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,           \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,           \
    _61, _62, _63, _64, _65, _66, _67, _68, _69, _70,           \
    _71, _72, _73, _74, _75, _76, _77, _78, _79, _80,           \
    _81, _82, _83, _84, _85, _86, _87, _88, _89, _90,           \
    _91, _92, _93, _94, _95, _96, _97, _98, _99, _100,          \
    _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, \
    _111, _112, _113, _114, _115, _116, _117, _118, _119, _120, \
    _121, _122, _123, _124, _125, _126, _127, N, ...) N
#define PP_RSEQ_N()                                       \
    127, 126, 125, 124, 123, 122, 121, 120,               \
        119, 118, 117, 116, 115, 114, 113, 112, 111, 110, \
        109, 108, 107, 106, 105, 104, 103, 102, 101, 100, \
        99, 98, 97, 96, 95, 94, 93, 92, 91, 90,           \
        89, 88, 87, 86, 85, 84, 83, 82, 81, 80,           \
        79, 78, 77, 76, 75, 74, 73, 72, 71, 70,           \
        69, 68, 67, 66, 65, 64, 63, 62, 61, 60,           \
        59, 58, 57, 56, 55, 54, 53, 52, 51, 50,           \
        49, 48, 47, 46, 45, 44, 43, 42, 41, 40,           \
        39, 38, 37, 36, 35, 34, 33, 32, 31, 30,           \
        29, 28, 27, 26, 25, 24, 23, 22, 21, 20,           \
        19, 18, 17, 16, 15, 14, 13, 12, 11, 10,           \
        9, 8, 7, 6, 5, 4, 3, 2, 1, 0

////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct __cstr
{
    bool garbage;
    size_t len;
    char * data;
} __cstr;
typedef __cstr  *str;

str __btos_lev1(bool b);                     // bool to str
str __ctos_lev1(char c);                    // char to str
str __itos_lev1(long long i);              // integers to str
str __ftos_lev1(double d);                // floats to str
str __stos_lev1(const char * const s);   // char * "strings" to str
str __str__lev1(str __s);               // decoy function to return str
const char* __cstr_lev1(__cstr cstr);  // decoy function to return str.data

str __strNret(size_t num, ...);                         // returns final str on multiple str chunks
const char *__cpNret(size_t num, const char* cp, ...); // returns const char * to .data field of str

////////////////////////////////////////////////////////////////////////////////////////////////////

#define __lev1(S) _Generic((S),         _Bool:              __btos_lev1,    \
    char:               __ctos_lev1,    unsigned char:      __itos_lev1,    \
    signed char:        __itos_lev1,    short:              __itos_lev1,    \
    unsigned short:     __itos_lev1,    int:                __itos_lev1,    \
    unsigned int:       __itos_lev1,    long:               __itos_lev1,    \
    unsigned long:      __itos_lev1,    unsigned long long: __itos_lev1,    \
    long long:          __itos_lev1,    float:              __ftos_lev1,    \
    double:             __ftos_lev1,    long double:        __ftos_lev1,    \
    const char* const:  __stos_lev1,    char*:              __stos_lev1,    \
    const char*:        __stos_lev1,    char* const:        __stos_lev1,    \
    str:                __str__lev1,    __cstr:             __cstr_lev1     \
)(S)

#define __lev2(...) MAP_LIST(__lev1, __VA_ARGS__)

#define __lev3(S, ...) _Generic((S),                                        \
    str:                __strNret,                                          \
    const char*:        __cpNret                                            \
)(PP_NARG(S, ##__VA_ARGS__), S, ##__VA_ARGS__)

#define str(...) __lev3(__lev2(__VA_ARGS__))

////////////////////////////////////////////////////////////////////////////////////////////////////

str __btos_lev1(bool b)
{
    size_t len = snprintf(NULL, 0, "%s", (b) ? "true" : "false");
    void* __s = calloc(1, sizeof(__cstr) + len + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str)__s)->garbage = true;
    ((str)__s)->len = len;
    snprintf(p, len + 1, "%s", (b) ? "true" : "false");
    ((str)__s)->data = p;

    return (str)__s;
}

str __ctos_lev1(char c)
{
    size_t len = snprintf(NULL, 0, "%c", c);
    void* __s = calloc(1, sizeof(__cstr) + len + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str)__s)->garbage = true;
    ((str)__s)->len = len;
    snprintf(p, len + 1, "%c", c);
    ((str)__s)->data = p;

    return (str)__s;
}

str __itos_lev1(long long i)
{
    size_t len = snprintf(NULL, 0, "%lld", i);
    void* __s = calloc(1, sizeof(__cstr) + len + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str)__s)->garbage = true;
    ((str)__s)->len = len;
    snprintf(p, len + 1, "%lld", i);
    ((str)__s)->data = p;

    return (str)__s;
}

str __ftos_lev1(double d)
{
    size_t len = snprintf(NULL, 0, "%f", d);
    void* __s = calloc(1, sizeof(__cstr) + len + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str)__s)->garbage = true;
    ((str)__s)->len = len;
    snprintf(p, len + 1, "%f", d);
    ((str)__s)->data = p;

    return (str)__s;
}

str __stos_lev1(const char *const s)
{
    size_t len = snprintf(NULL, 0, "%s", s);
    void* __s = calloc(1, sizeof(__cstr) + len + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str )__s)->garbage = true;
    ((str)__s)->len = len;
    snprintf(p, len + 1, "%s", s);
    ((str)__s)->data = p;

    return (str)__s;
}

str __str__lev1(str __s)
{
    return (str)__s;
}

const char *__cstr_lev1(__cstr cstr)
{
    return (const char *)cstr.data;
}

///////////////////////////////////////////////////////////

str __strNret(size_t num, ...)
{
    va_list args;
    va_start(args, num);
    size_t len = 0;
    size_t index = 0;

    for (int i = 0; i < num; ++i)
    {
        str __tmpstr = va_arg(args, str);
        len += __tmpstr->len;
    }
    va_end(args);

    void* __s = calloc(1, sizeof(__cstr) + len + 1);
    char *temp = (char *)(__s + sizeof(__cstr));

    va_start(args, num);
    str __tmpstr = va_arg(args, str);
    strncpy(temp, __tmpstr->data, __tmpstr->len);
    index += __tmpstr->len;
    if (__tmpstr->garbage)
    {
        free(__tmpstr);
    }
    __tmpstr = NULL;

    for (int i = 1; i < num; ++i)
    {
        __tmpstr = va_arg(args, str);
        strncpy(temp + index, __tmpstr->data, __tmpstr->len);
        index += __tmpstr->len;
        if (__tmpstr->garbage)
        {
            free(__tmpstr);
        }
    }
    __tmpstr = NULL;

    va_end(args);

    ((str)__s)->garbage = false;
    ((str)__s)->len = len;
    ((str)__s)->data = temp;

    return (str)__s;
}

const char *__cpNret(size_t num, const char *cp, ...)
{
    return (const char *)cp;
}

#endif
