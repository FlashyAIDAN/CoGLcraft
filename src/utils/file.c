#include "file.h"

// Mainly from https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
char *ReadFile(const char *filename)
{
    char * buffer = 0;
    long length;
    FILE * f = fopen (filename, "rb");

    if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = malloc (length);
      if (buffer)
      {
        fread (buffer, 1, length, f);
      }
      fclose (f);

      buffer[length] = '\0';
    }

    return buffer;
}