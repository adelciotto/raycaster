#include <stdlib.h>
#include "util.h"

int Util_ParseInt(const char *str) {
    return (int)strtol(str, (char **)NULL, 10);
}
