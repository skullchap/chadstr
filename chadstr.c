#include "chadstr.h"

str __btos_lev1(bool b)
{
    size_t len = (b) ? 4 : 5;
    void *__s = malloc(sizeof(__cstr) + len + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str)__s)->garbage = true;
    ((str)__s)->len = len;
    p = (b) ? "true" : "false";
    p[len] = '\0';

    ((str)__s)->data = p;

    return (str)__s;
}

str __ctos_lev1(char c)
{
    size_t len = 1;
    void *__s = malloc(sizeof(__cstr) + len + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str)__s)->garbage = true;
    ((str)__s)->len = len;
    p[0] = c;
    p[len] = '\0';
    ((str)__s)->data = p;

    return (str)__s;
}

str __itos_lev1(long long i)
{
    size_t len = snprintf(NULL, 0, "%lld", i);
    void *__s = malloc(sizeof(__cstr) + len + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str)__s)->garbage = true;
    ((str)__s)->len = len;
    snprintf(p, len + 1, "%lld", i);
    p[len] = '\0';
    ((str)__s)->data = p;

    return (str)__s;
}

str __ftos_lev1(double d)
{
    size_t len = snprintf(NULL, 0, "%f", d);
    void *__s = malloc(sizeof(__cstr) + len + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str)__s)->garbage = true;
    ((str)__s)->len = len;
    snprintf(p, len + 1, "%f", d);
    p[len] = '\0';
    ((str)__s)->data = p;

    return (str)__s;
}

str __stos_lev1(const char *const s)
{
    size_t len = strlen(s);
    void *__s = malloc(sizeof(__cstr) + len + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str)__s)->garbage = true;
    ((str)__s)->len = len;
    memcpy(p, s, len + 1);
    ((str)__s)->data = p;

    return (str)__s;
}

str __vtos_lev1(void *ptr)
{
    void *__s = malloc(sizeof(__cstr) + 1);
    char *p = (char *)(__s + sizeof(__cstr));

    p[0] = '\0';

    ((str)__s)->garbage = true;
    ((str)__s)->len = 0;
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

    void *__s = malloc(sizeof(__cstr) + len + 1);
    char *temp = (char *)(__s + sizeof(__cstr));

    va_start(args, num);
    str __tmpstr = va_arg(args, str);
    memcpy(temp, __tmpstr->data, __tmpstr->len);
    index += __tmpstr->len;
    if (__tmpstr->garbage)
    {
        free(__tmpstr);
    }
    __tmpstr = NULL;

    for (int i = 1; i < num; ++i)
    {
        __tmpstr = va_arg(args, str);
        memcpy(temp + index, __tmpstr->data, __tmpstr->len);
        index += __tmpstr->len;
        if (__tmpstr->garbage)
        {
            free(__tmpstr);
        }
    }
    __tmpstr = NULL;
    temp[len] = '\0';

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

__cmdt __pcmd_lev1(cmd cmd_)
{
    return cmd_;
}

str __cmdNret(size_t num, __cmdt cmd_, ...)
{
    va_list args;
    va_start(args, cmd_);

    str __cmd_head = str(cmd_.cmd);
    str __cmd_body = str(" ");

    for (int i = 1; i < num; ++i)
    {
        str __tmpstr = va_arg(args, str);
        str __cmd_body_old = __cmd_body;
        __cmd_body = str(__cmd_body, __tmpstr);
        free(__cmd_body_old);
    }
    va_end(args);

    str __cmd_ = str(__cmd_head, __cmd_body);
    free(__cmd_head);
    free(__cmd_body);

    FILE *__exec_cmd = popen(str(*__cmd_), "r");
    if (__exec_cmd == NULL)
    {
        fprintf(stderr, "error launching cmd\n");
        return str(NULL);
    }

    size_t BUFLEN = 8192;

    char *__buf = calloc(1, BUFLEN);
    if (__buf == NULL)
    {
        fprintf(stderr, "buf malloc error\n");
        return str(NULL);
    }

    size_t __bytelen = 0;

    while (__bytelen += fread(__buf, 1, BUFLEN, __exec_cmd))
    {
        if (__bytelen < BUFLEN)
            break;
        __buf = realloc(__buf, BUFLEN += BUFLEN);

    }
    pclose(__exec_cmd);

    void *__s = calloc(1, sizeof(__cstr) + __bytelen);
    char *p = (char *)(__s + sizeof(__cstr));

    ((str)__s)->garbage = false;
    ((str)__s)->len = __bytelen - 1;
    ((str)__s)->data = p;

    strncpy(p, __buf, __bytelen);

    free(__buf);
    free(__cmd_);

    return __s;
}

/*
 * If you are comfortable with range starting at index 1,
 * #define HUMAN_RANGE before #include "chadstr.h"
 * Note: negative end indices are still in "human" format 
 * starting at 1 no matter HUMAN_RANGE defined or not.
*/

str __str_range(str __s_in, long long start, long long end)
{
#define HUMAN_SHIFT 0
#ifdef HUMAN_RANGE
#undef HUMAN_SHIFT
#define HUMAN_SHIFT 1
#endif

    size_t __strlen = __s_in->len;

    if (end < 0)
        end = __strlen + end + HUMAN_SHIFT;

    if (start < 0)
        start = 0;

    long long __abs = (end - start > 0)
                          ? end - start
                          : -1 * (end - start);

    if (__abs > __strlen)
    {
        return str(NULL);
    }

#ifdef HUMAN_RANGE

    if (start == 0)
        start = 1;

#endif

    if (start > end) // swap
    {

        char *__pstart = __s_in->data;
        char *__pend = __s_in->data + __strlen - 1;

        while (__pstart < __pend)
        {
            XOR_SWAP(*__pstart, *__pend);
            __pstart++;
            __pend--;
        }
    }

    char *temp = calloc(1, __abs + 2);
    strncpy(temp, __s_in->data + start - HUMAN_SHIFT, __abs + 1);

#undef HUMAN_SHIFT

    str __s_out = str(temp);
    __s_out->garbage = true;
    free(temp);

    return __s_out;
}
