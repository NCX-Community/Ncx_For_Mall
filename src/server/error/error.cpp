#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void errif(bool condition, const char* msg) {
    if (condition) {
        perror(msg);
        exit(1);
    }
}