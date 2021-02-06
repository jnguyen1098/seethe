#ifndef SEETHE_H
#define SEETHE_H

#include <stdio.h>
#include <time.h>

/* Default level */
#ifndef LOG_LEVEL
    #define LOG_LEVEL   WARNING
#endif

/* Colour customization */
#define DEBUG_COLOUR    ""
#define INFO_COLOUR     "\x1B[36m"
#define NOTICE_COLOUR   "\x1B[32;1m"
#define WARNING_COLOUR  "\x1B[33m"
#define ERROR_COLOUR    "\x1B[31m"
#define CRITICAL_COLOUR "\x1B[41;1m"

/* Do not change this. */
#define RESET_COLOUR    "\x1B[0m"

/* Formatting prefs. */
#define MSG_ENDING      "\n"
#define TIME_FORMAT     "%T"
#define BORDER          "-"

/* Enabler flags */
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

/* Log to screen */
#define emit_log(colour, level, file, func, line, ...) do {                         \
                                                                                    \
    /* notate the time */                                                           \
    time_t raw_time = time(NULL);                                                   \
    char time_buffer[80];                                                           \
    strftime(time_buffer, 80, TIME_FORMAT, localtime(&raw_time));                   \
                                                                                    \
    /* enable colour */                                                             \
    printf("%s", DISPLAY_COLOUR ? colour : "");                                     \
                                                                                    \
    /* display the time */                                                          \
    printf("%s%s", DISPLAY_TIME ? time_buffer : "", DISPLAY_TIME ? " " : "");       \
                                                                                    \
    /* display the level */                                                         \
    printf("%10s%s", DISPLAY_LEVEL ? level : "", DISPLAY_LEVEL ? " " : "");         \
                                                                                    \
    /* display the function doing the logging */                                    \
    printf("%s%s", DISPLAY_FUNC ? func : "", DISPLAY_FUNC ? " " : "");              \
                                                                                    \
    /* display the file and/or the line number */                                   \
    printf(                                                                         \
        "%s%s%s%.d%s%s",                                                            \
        DISPLAY_FUNC && (DISPLAY_FILE || DISPLAY_LINE) ? "(" : "",                  \
        DISPLAY_FILE ? file : "",                                                   \
        DISPLAY_FILE && DISPLAY_LINE ? ":" : "",                                    \
        DISPLAY_LINE ? line : 0,                                                    \
        DISPLAY_FUNC && (DISPLAY_FILE || DISPLAY_LINE) ? ") " : "",                 \
        !DISPLAY_FUNC && (DISPLAY_FILE || DISPLAY_LINE) ? " " : ""                  \
    );                                                                              \
                                                                                    \
    /* display message border */                                                    \
    printf("%s%s", DISPLAY_BORDER ? BORDER : "", DISPLAY_BORDER ? " " : "");        \
                                                                                    \
    /* display the callee's message */                                              \
    if (DISPLAY_MESSAGE) printf(__VA_ARGS__);                                       \
                                                                                    \
    /* add the message ending (usually '\n') */                                     \
    printf("%s", DISPLAY_ENDING ? MSG_ENDING : "");                                 \
                                                                                    \
    /* reset the colour */                                                          \
    printf("%s", DISPLAY_RESET ? RESET_COLOUR : "");                                \
                                                                                    \
} while (0)

/* Level enum */
#define DEBUG       0
#define INFO        1
#define NOTICE      2
#define WARNING     3
#define ERROR       4
#define CRITICAL    5
#define SILENT      6

/* DEBUG LOG */
#define debug(...) do {                                                             \
    if (LOG_LEVEL == DEBUG) {                                                       \
        emit_log(                                                                   \
            DEBUG_COLOUR, "[DEBUG]", __FILE__, __func__, __LINE__, __VA_ARGS__      \
        );                                                                          \
    }                                                                               \
} while (0)

/* INFO LOG */
#define info(...) do {                                                              \
    if (LOG_LEVEL <= INFO) {                                                        \
        emit_log(                                                                   \
            INFO_COLOUR, "[INFO]", __FILE__, __func__, __LINE__, __VA_ARGS__        \
        );                                                                          \
    }                                                                               \
} while (0)

/* NOTICE LOG */
#define notice(...) do {                                                            \
    if (LOG_LEVEL <= NOTICE) {                                                      \
        emit_log(                                                                   \
            NOTICE_COLOUR, "[NOTICE]", __FILE__, __func__, __LINE__, __VA_ARGS__    \
        );                                                                          \
    }                                                                               \
} while (0)

/* WARNING LOG */
#define warning(...) do {                                                           \
    if (LOG_LEVEL <= WARNING) {                                                     \
        emit_log(                                                                   \
            WARNING_COLOUR, "[WARNING]", __FILE__, __func__, __LINE__, __VA_ARGS__  \
        );                                                                          \
    }                                                                               \
} while (0)

/* ERROR LOG */
#define error(...) do {                                                             \
    if (LOG_LEVEL <= ERROR) {                                                       \
        emit_log(                                                                   \
            ERROR_COLOUR, "[ERROR]", __FILE__, __func__, __LINE__, __VA_ARGS__      \
        );                                                                          \
    }                                                                               \
} while (0)

/* CRITICAL LOG */
#define critical(...) do {                                                          \
    if (LOG_LEVEL <= CRITICAL) {                                                    \
        emit_log(                                                                   \
            CRITICAL_COLOUR, "[CRITICAL]", __FILE__, __func__, __LINE__, __VA_ARGS__\
        );                                                                          \
    }                                                                               \
} while (0)

#endif // seethe.h
