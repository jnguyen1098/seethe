# Seethe

<center>
<img src="https://i.imgur.com/6CxeynF.gif">
</center>

Logging so simple, you only need to include a single header file. seethe supports 6 different log levels (`DEBUG`, `INFO`, `NOTICE`, `WARNING`, `ERROR`, and `CRITICAL`) as well as variadic printing (à la `printf` and friends).

## Installation and Usage

Just `#include "seethe.h"` in the file you're using. It's that simple.

```
#include "seethe.h"
#include <stdio.h>

int main(void) {
    int x = 10;
    int y = 11;
    if (x != y) {
        critical("Something is afoot...");
    }
    return 0;
}
```

### Locally Changing the Log Level

To change the warning level for a particular point in a particular file, re-`#define` the pre-processor macro at the point you'd like:

```
...
#define LOG_LEVEL   WARNING
warning("first warning");
#define LOG_LEVEL   ERROR     // now only ERROR and CRITICAL are visible now
warning("second warning");
...
```

output:

```
21:59:28 [WARNING] runner.c:37 - first warning
21:59:28 [ERROR] runner.c:40 - some error
```

(If you want to suppress the 'redefinition' error from `gcc`, put `#undef LOG_LEVEL` before your `#define`).

### Globally Changing the Log Level

If you'd like to set a global `LOG_LEVEL` for your entire project, compile the `LOG_LEVEL` as a `gcc` flag:

```
gcc *.c -D LOG_LEVEL=ERROR
```