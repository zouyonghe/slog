# SLOG

Structured logging library for C.

## Features

- Header-only C library (no dependencies)
- Structured JSON logging
- Thread-safe with memory pools
- Multiple data types support
- Auto escaping & timestamps
- Log level filtering

## Tutorial

```c
#include "slog.h"

int main() {
    // Message
    SLOG(SLOG_INFO, "Hello World");

    // Types
    SLOG(SLOG_INFO, "User information",
        SLOG_STRING("name", "qaqland"),
        SLOG_INT("age", 25),
        SLOG_FLOAT("score", 96.5),
        SLOG_BOOL("active", true));

    // Nested objects
    SLOG(SLOG_INFO, "Complex data structure",
        SLOG_OBJECT("user",
            SLOG_STRING("name", "qaqland"),
            SLOG_OBJECT("profile",
                SLOG_STRING("email", "qaq@qaq.land"),
                SLOG_INT("id", 114514))));

    // Minimum log level
    SLOG_SET_LEVEL(SLOG_INFO);
    SLOG(SLOG_DEBUG, "this will be filtered");

    SLOG_FREE();
    return 0;
}
```

## Development

```bash
meson setup build
meson compile -C build
meson test -C build
meson test -C build --wrapper='valgrind'

./format.sh
```

## Acknowledgements

Special thanks to [@XJJ](https://github.com/mivinci) for the valuable
contribution and support.

## License

Mozilla Public License 2.0
