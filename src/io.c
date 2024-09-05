#include <stdio.h>
#include "../include/io.h"

char *get_contents(const char *fileName)
{
    FILE *file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("Error reading file %s", fileName);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = calloc(length + 1, sizeof(char));

    if (!buffer)
    {
        printf("Error reading buffer");
        exit(1);
    }

    fread(buffer, sizeof(char), length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

bool has_valid_extension(const char *fileName)
{
    if (fileName == NULL || strlen(fileName) < 3)
    {
        return false;
    }

    size_t len = strlen(fileName);
    const char *ext = fileName + (len - 3);

    return strcmp(ext, EXT) == 0;
}
