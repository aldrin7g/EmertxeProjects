#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum{
    failure,
    success
} Status;

typedef enum{
    unsupported,
    encode,
    decode
} OperationType;

#endif