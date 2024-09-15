#include "utils.h"

void error_info(bool condition, const char* message)
{
    if (condition)
    {
        perror(message);
        exit(EXIT_FAILURE);
    }
}