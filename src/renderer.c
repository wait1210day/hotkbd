#include "renderer.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

FILE *_dri_err_out_ = NULL;
FILE *_dri_tri_out_ = NULL;
FILE *_dri_war_out_ = NULL;
FILE *_dri_caught_out_ = NULL;
FILE *_dri_int_out_ = NULL;
FILE *_dri_exception_out = NULL;
FILE *_dri_info_out_ = NULL;

void driInit()
{
    _dri_err_out_ = stderr;
    _dri_tri_out_ = stdout;
    _dri_war_out_ = stderr;
    _dri_caught_out_ = stdout;
    _dri_int_out_ = stderr;
    _dri_exception_out = stderr;
    _dri_info_out_ = stdout;
}

void driRedirector(unsigned short target, FILE *stream)
{
    if (stream == NULL)
        machineException ("NullPointerException", "pointer");

    switch (target)
    {
    case DCR_ERROR:
        _dri_err_out_ = stream;
        break;
    case DCR_TRIGGER:
        _dri_tri_out_ = stream;
        break;
    case DCR_WARNING:
        _dri_war_out_ = stream;
        break;
    case DCR_CAUGHT:
        _dri_caught_out_ = stream;
        break;
    case DCR_INT:
        _dri_int_out_ = stream;
        break;
    case DCR_ECPT:
        _dri_exception_out = stream;
        break;
    case DCR_INFO:
        _dri_info_out_ = stream;
        break;

    default:
        break;
    }
}

void dri(unsigned short type, char *fmt, ...)
{
    char buf[4096];

    va_list valist;
    va_start (valist, fmt);
    vsprintf (buf, fmt, valist);

    switch (type)
    {
    case DCR_ERROR:
        fprintf (_dri_err_out_, "\033[35;1mhotkbd:\033[0m \033[31;1merror:\033[0m %s", buf);
        break;
    case DCR_TRIGGER:
        fprintf (_dri_tri_out_, "\033[35;1mhotkbd:\033[0m \033[32;1mtrigger:\033[0m %s", buf);
        break;
    case DCR_WARNING:
        fprintf (_dri_war_out_, "\033[35;1mhotkbd:\033[0m \033[33;1mwarning:\033[0m %s", buf);
        break;
    case DCR_CAUGHT:
        fprintf (_dri_caught_out_, "\033[35;1mhotkbd:\033[0m \033[34;1mcaught:\033[0m %s", buf);
        break;
    case DCR_INT:
        fprintf (_dri_int_out_, "\033[35;1mhotkbd:\033[0m \033[36;1minterrupted:\033[0m %s", buf);
        break;
    case DCR_ECPT:
        fprintf (_dri_exception_out, "\033[35;1mhotkbd:\033[0m \033[37;1mexception:\033[0m %s", buf);
        break;
    case DCR_INFO:
        fprintf (_dri_info_out_, "\033[35;1mhotkbd:\033[0m \033[32;1minfo:\033[0m %s", buf);
        break;

    default:
        break;
    }

    va_end (valist);
}

void machineException (char const *exceptionStr, char const *code)
{
    dri (
        DCR_ECPT,
        "process aborted by \033[32;1m%s\033[31;1m<\033[34;1m%s\033[31;1m>\033[0m\n",
        exceptionStr,
        code
    );
    signalInterrupt (-1);
}