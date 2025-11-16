# C-project
Repository for project required in ICS0004

## Building the project
To build this program, `CMake` is required.

First, create build directory:
```bash
cmake -B build
```
Next, build the program:
```bash
cmake --build build
```

This should generate the executable in the root directory.

## TODO
### High Priority
- Implement storage(use [`storage.c`](./src/storage.c) for the back end, [`app.c`](./src/app.c) for middle man and [`ui.c`](./src/ui.c) for the front end)
- Fill out header files

### Low Priority
- Re-introduce password for admin, this time with password being read from `.env` file
