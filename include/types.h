#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

/* Definizione del tipo di dati supportati */
typedef enum
{
    TYPE_INT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_DATE,
    TYPE_TIME
} DataType;

/* Definizione del valore generico */
typedef struct
{
    DataType type;
    union
    {
        int intValue;
        char *stringValue;
        bool boolValue;
        float floatValue;
        double doubleValue;
        struct
        {
            int year, month, day;
        } dateValue;
        struct
        {
            int hour, minute, second;
        } timeValue;
    } data;
} Value;

#endif