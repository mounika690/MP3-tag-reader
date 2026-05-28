#ifndef TYPES_H
#define TYPES_H

#include<stdio.h>



/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_view,
    e_edit,
    e_unsupported
} OperationType;




/* Color codes for terminal output */
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define RESET   "\033[0m"


#endif
