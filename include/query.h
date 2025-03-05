#ifndef QUERY_H
#define QUERY_H

#include "table.h"

RowLink *executeQuery(Table *table, const char *query, int *count);

#endif
