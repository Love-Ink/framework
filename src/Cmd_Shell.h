#ifndef __CMD_SHELL_H
#define __CMD_SHELL_H

#include <stdio.h>
#include <stdint.h>

typedef void (*pfunc)(int argc, char** argv);

typedef struct Cmd_Format_Str {

	char Format[20];
	char Val[20];
    uint8_t Val_Type;
    struct Cmd_Format_Str* next;

}Cmd_Format_init;

typedef struct Cmd_Keyword_Str {

	char *Cmd_Keyword;
    char *Cmd_help_arg;
    char *Cmd_format_arg;
    pfunc pFuncCommand;
    uint8_t Format_Count;
    Cmd_Format_init* Cmd_Format;

    struct Cmd_Keyword_Str* next;

}Cmd_Str;

#define CMD_OK  0
#define CMD_NOT_FOUND   -1
#define CMD_FUNC_BUT_FORMAT_ERR  -2

#ifdef __cplusplus 
extern "C" {
#endif

    extern const char *logo;

    void Shell_KeywordAdd(const char* keyword, pfunc pfuncCmd);
    void Shell_FormatAdd(const char* keyword, const char* format);
    void Shell_HelpArgAdd(const char* keyword, const char* help_arg);
    int Shell_Keyparse(const char* buf);

#ifdef __cplusplus 
}
#endif

#endif