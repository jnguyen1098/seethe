# Seethe

<p align="center">
    <img alt="demonstration" src="https://i.imgur.com/PLJjHmP.png" width="100%">
</p>

Logging so simple, you only need to include a single header file. seethe supports 6 different log levels (`DEBUG`, `INFO`, `NOTICE`, `WARNING`, `ERROR`, and `CRITICAL`) as well as variadic printing (à la `printf` and friends).

<p align="center">
    <img alt="demonstration" src="https://s2.gifyu.com/images/hGbWMYo1Qn.gif" width="80%">
</p>

## Installation and Usage

Just `#include "seethe.h"` in the file you're using and use any of the 6 macros (look at `tests.c`/`run_tests.sh` for examples). It's that simple.

```c
#include "seethe.h"
#include <stdio.h>

int main(void) {
    debug("debug log");
    info("informational log");
    notice("notice log");
    warning("warning log");
    error("error log");
    critical("critical log");
}
```

### Locally Changing the Log Level

To change the warning level for a particular point in a particular file, re-`#define` the pre-processor macro at the point you'd like:

```c
...
#define LOG_LEVEL   WARNING
warning("first warning");
#define LOG_LEVEL   ERROR     // now only ERROR and CRITICAL are visible
warning("second warning");
error("some error");
...
```

```c
21:59:28 [WARNING] runner.c:37 - first warning
21:59:28 [ERROR] runner.c:40 - some error
```

(If you want to suppress the 'redefinition' error from `gcc`, put `#undef LOG_LEVEL` before your `#define`).

### Globally Changing the Log Level

If you'd like to set a global `LOG_LEVEL` for your entire project, compile the `LOG_LEVEL` as a `gcc` flag:

```bash
gcc *.c -D LOG_LEVEL=ERROR
```

### Configuration

I tried making seethe as configurable as possible, but when everything's in a header file, that might be a little difficult.

- **Colour** — can be re-configured using VT100 ANSI escape codes. The defaults are:

```c
#define DEBUG_COLOUR    ""
#define INFO_COLOUR     "\x1B[36m"
#define NOTICE_COLOUR   "\x1B[32;1m"
#define WARNING_COLOUR  "\x1B[33m"
#define ERROR_COLOUR    "\x1B[31m"
#define CRITICAL_COLOUR "\x1B[41;1m"
```

- **Message ending** — by default, every log message has a newline (`\n`) appended to it in order to give `puts`-like syntax (i.e. for those who hate typing `\n` for every print statement). To remove the trailing newline and restore full `printf` functionality, change `"\n"` to `""`. The default is:

```c
#define MSG_ENDING      "\n"
```

- **Time format** — each log message has a timestamp attached to it, defined from the `strftime` function. It can be changed (though if you are making a particularly long message, you should probably up the buffer from 80 `char`s). The default is:

```c
#define TIME_FORMAT     "%T "
```

- **Logger format** — this isn't very friendly to work with, but the `emit_log` macro is where the actual log printing takes place. 