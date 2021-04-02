![](https://i.imgur.com/3u3YAsz.png)

# Seethe

Logging so simple, you only need to include a single header file. seethe supports 6 different log levels (`DEBUG`, `INFO`, `NOTICE`, `WARNING`, `ERROR`, and `CRITICAL`), a `SILENT` mode as well as `printf`-style variadic printing.

<p align="center">
    <img alt="demonstration" src="https://s2.gifyu.com/images/hGbWMYo1Qn.gif">
</p>

## Installation and Usage

Just `#include "seethe.h"` in the file you're using and use any of the 6 macros. It's that simple. Look at the below sample code or `example.c` for more boilerplate.

```c
#include "seethe.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

    // all the available macros
    debug("debug log");
    info("informational log");
    notice("notice log");
    warning("warning log");
    error("error log");
    critical("critical log");

    // sample usage
    info("starting program");
    char *my_string;
    if (!(my_string = malloc(BUFSIZ * sizeof(char)))) {
        critical("failed to make size-%zu buffer. exiting\n", BUFSIZ);
        abort();
    }
    debug("getting input from user");
    printf("Enter some stuff: ");
    while (fgets(my_string, BUFSIZ, stdin) != NULL) {
        if (!strcmp(my_string, "\n")) {
            info("user entered blank input. exiting");
            return 0;
        }
    }
    error("fgets failed to return a proper result");
    return 1;

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

To set the logging level for an entire file, define the `LOG_LEVEL` before `#include`'ing seeth.

```c
#define LOG_LEVEL   DEBUG
#include "seethe.h"
...
```

### Silencing Log Output

The `SILENT` log level is provided for complete suppression of all warnings.

```c
#define LOG_LEVEL   SILENT
#include "seethe.h"
...
```

### Globally Changing the Log Level

If you'd like to set a global `LOG_LEVEL` for your entire project, compile the `LOG_LEVEL` as a `gcc` flag:

```bash
gcc *.c -D LOG_LEVEL=ERROR
```

## Customization

I tried making seethe as configurable as possible, though there's still room for improvement.

### Colour

It is possible to configure the different level colours using VT100 ANSI escape codes. The defaults are:

```c
#define DEBUG_COLOUR    ""
#define INFO_COLOUR     "\x1B[36m"
#define NOTICE_COLOUR   "\x1B[32;1m"
#define WARNING_COLOUR  "\x1B[33m"
#define ERROR_COLOUR    "\x1B[31m"
#define CRITICAL_COLOUR "\x1B[41;1m"
```

### Message Endings

By default, every log message has a newline (`\n`) appended to it in order to give `puts`-like syntax (i.e. for those who hate typing `\n` for every print statement). To remove the trailing newline and restore full `printf` functionality, change `"\n"` to `""`. The default is:

```c
#define MSG_ENDING      "\n"
```

### Time Format

Each log message has a timestamp attached to it, defined from the `strftime` function. It can be changed (though if you are making a particularly long message, you should probably up the buffer from 80 `char`s). The default is:

```c
#define TIME_FORMAT     "%T "
```

### Message Border

You can configure the border that shows up before the actual message (by default it's `-`). The default is:

```c
#define BORDER          "-"
```

### Logger Format

There are two ways to customize this: there are flags that can toggle existing components and I tried my best to make it easy(-ish) to make new ones.

- **Enabler flags** — toggles for the logger message features. By default, everything is enabled (`=1`). To turn something off, change it to `0`. Do not remove the macro. The built-in flags are:

    - `DISPLAY_COLOUR` — enable colour-coding the log messages based on level
    - `DISPLAY_TIME` — output the time for each log event (date should be configurable)
    - `DISPLAY_LEVEL` — display the logging level of each message
    - `DISPLAY_FUNC` — display the calling function for each log message
    - `DISPLAY_FILE` — print the file from which the log message originates from
    - `DISPLAY_LINE` — print the line from which the log message originates from
    - `DISPLAY_BORDER` — print the border that comes before the messages
    - `DISPLAY_MESSAGE` — print the log message
    - `DISPLAY_ENDING` — print the message ending (by default it's `\n`)
    - `DISPLAY_RESET` — reset the message colour-coding at the end of each log

    It is not recommended to disable `DISPLAY_ENDING` or `DISPLAY_RESET`.

    In the library, the flags are arranged like so:

    ```c
    #define DISPLAY_COLOUR  1
    #define DISPLAY_TIME    1
    #define DISPLAY_LEVEL   1
    #define DISPLAY_FUNC    1
    #define DISPLAY_FILE    1
    #define DISPLAY_LINE    1
    #define DISPLAY_BORDER  1
    #define DISPLAY_MESSAGE 1
    #define DISPLAY_ENDING  1
    #define DISPLAY_RESET   1
    ```

- **Message components** — the actual components that make up the log messages. Do note that although you can rearrange them freely, it is important to follow the same formatting structure as the existing components when creating new ones. This means if a component is removed, so will its spacing (hence why a lot of the existing components have a second `%s` in the `printf`—the extra space needs to be able to seamlessly appear and disappear for modularity and toggling).

    For a lot of components, it's a very simple check:

    ```c
    printf("%s%s", DISPLAY_FUNC ? func : "", DISPLAY_FUNC ? " " : "");
    ```

    Do you need to display the calling function? If so, print a space along with it. Otherwise, don't print anything (not even the space).

    But for some others, it is a lot more complex. Take a look at this monstrosity here:

    ```c
    printf(                                                                         \
        "%s%s%s%.d%s%s",                                                            \
        DISPLAY_FUNC && (DISPLAY_FILE || DISPLAY_LINE) ? "(" : "",                  \
        DISPLAY_FILE ? file : "",                                                   \
        DISPLAY_FILE && DISPLAY_LINE ? ":" : "",                                    \
        DISPLAY_LINE ? line : 0,                                                    \
        DISPLAY_FUNC && (DISPLAY_FILE || DISPLAY_LINE) ? ") " : "",                 \
        !DISPLAY_FUNC && (DISPLAY_FILE || DISPLAY_LINE) ? " " : ""                  \
    );
    ```

    This part of the `emit_log` macro is responsible for the part of the logger output that emits the calling function, source file, and line number (for example `main (example.c:24)`).
    
    The left and right brackets (`(`/`)`) are only printed when the actual function itself is printed and either source file and/or line are also printed (otherwise you'd get lines like `04:44:32  [WARNING] (example.c:27) - warning log` where don't need the brackets, or `04:44:32  [WARNING] main () - warning log` where neither file nor line are printed, resulting in empty brackets being printed next to the function). 

    The second last print specifier is responsible for printing a space after the right bracket (`)`) in the case that the logger is configured to print the calling function. However, in the case that brackets aren't printed at all, we still need a space to trail the entire component, hence the `!DISPLAY_FUNC && (DISPLAY_FILE || DISPLAY_LINE) ? " " : ""` construct.

    As a result of this over-engineered mess, seethe can dynamically output:

    - `main (example.c:24)`

    - `example.c:24`

    - `main (24)`

    - `main`

    - `24`

    Also, feel free to play around with the spacing on the `printf` print specifiers (I personally prefer `%10s%s` for level display).



## TODO?

- Allow for configuring print specifiers easily?

## License

ISC License. Do whatever the fuck you want.
