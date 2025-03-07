#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <setjmp.h>

extern jmp_buf env;
void handle_error(void);

#endif
