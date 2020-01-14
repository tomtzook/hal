#ifndef HAL_LOG_H
#define HAL_LOG_H

#include <stdio.h>

#define LOG(X...) \
    printf(X)

#define LOGLN(X...) \
    LOG(X); \
    printf("\n")

#endif //HAL_LOG_H
