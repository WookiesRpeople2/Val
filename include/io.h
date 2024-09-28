#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdbool.h>
#define MAX_PATH 1024
#define MAX_INPUT 64
#define VERSION "0.0.1"
#define EXT "val"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

inline void message_help()
{
    char path[MAX_PATH];
    if (GetModuleFileName(NULL, path, MAX_PATH) != 0)
    {
        printf("usage: %s fileName\n", path);
    }
    else
    {
        printf("Val is not installed\n");
    }
}
#elif defined(__linux__)
#include <limits.h>
#include <unistd.h>

inline void message_help()
{
    char path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len != -1 && len < sizeof(path))
    {
        path[len] = '\0'; // Null-terminate the string
        printf("usage: %s fileName\n", path);
    }
    else
    {
        fprintf(stderr, "Val is not installed\n");
    }
}

#elif defined(__APPLE__) && defined(__MACH__)
#include <mach-o/dyld.h>
#include <limits.h>

inline void message_help()
{
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
    {
        printf("usage: %s fileName\n", path);
    }
    else
    {
        fprintf(stderr, "Val is not installed\n");
    }
}

#else
#error "Unsupported platform"
#endif

char *get_contents(const char *fileName);

bool has_valid_extension(const char *fileName);

#endif