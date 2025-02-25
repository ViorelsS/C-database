# Feature da aggiungere:

1. Supporto per tipologie diverse di dato

   ```C
   /* Definizione del tipo di dati supportati */
   typedef enum
   {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_CHAR,
    TYPE_BOOL,
    TYPE_DATE,     /* Data nel formato: YYYY-MM-DD */
    TYPE_TIME,     /* Orario nel formato: HH:MM:SS */
    TYPE_DATETIME, /* Combinazione di data e orario */
    TYPE_BINRAY    /* Per i dati binari (immagini/file) */
   } DataType;

   /* Definizione del valore generico */
   typedef struct
   {
    DataType type;
    union
    {
        int intValue;
        float floatValue;
        double doubleValue;
        char *stringValue;
        char charValue;
        bool boolValue;
        struct
        {
            int year;
            int month;
            int day;
        } dateValue;
        struct
        {
            int hour;
            int minute;
            int second;
        } timeValue;
    } data;
   } Value;
   ```
