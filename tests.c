#define LOG_LEVEL   DEBUG

#include "seethe.h"
#include <stdio.h>

#define assert_int(x, y) do {   \
    if (x != y) \
        critical("%d and %d are not equal!", x, y); \
} while (0)

void fake_allocate(char *buf)
{
    if (!buf) {
        critical("Non-NULL argument expected");
        return;
    }

    *buf = 'D';
}

int main(void)
{
    // Smoke testing
    debug("debug log");
    info("informational log");
    warning("warning log");
    error("error log");
    critical("critical log");
    puts("");

    // Do note that function calls don't benefit from __LINE__
    // unless they are inlined using macros (unfortunately)
    info("declaring int 5 and int 6");
    int first = 5;
    int second = 6;
    assert_int(first, second);
    puts("");

    // Testing a function that relies on a non-NULL argument
    char *my_buf = NULL;
    fake_allocate(my_buf);
    puts("");

    // Self-diagnostic indicators
    debug("your logging level is DEBUG");
    info("your logging level is DEBUG or INFO");
    warning("your logging level is WARNING, INFO or DEBUG");
    error("your logging level is ERROR,  WARNING, INFO or DEBUG");
    critical("your logging level is CRITICAL, ERROR, WARNING, INFO or DEBUG");
    puts("");

    return 0;
}
