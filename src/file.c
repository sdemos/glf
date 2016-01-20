// file.c
//
// useful file operation abstractions
//
// author: Stephen Demos (stphndemos@gmail.com)

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

/**
 * get_file_contents
 *
 * get the contents of a file on disk given the filepath
 *
 * if any errors occur during the reading of the file on disk, including
 * opening the file, allocating memory, and actually reading the file, a
 * detailed error message is printed to stderr and exit(1) is called
 *
 * arguments:
 *  const char *filepath - the relative filepath of the file to return the
 *      contents of
 *
 * returns:
 *  a char * allocated with calloc to exactly the size of the file passed,
 *  containing the contents of the file
 */
char *get_file_contents (const char *filepath)
{
    FILE *fp;
    long  file_size;
    char *buffer;

    // open the file
    fp = fopen(filepath, "r");
    if (!fp) {
        perror("error: get_file_contents: fp fopen");
        fprintf(stderr, "error: get_file_contents: fp fopen problem child: %s\n", filepath);
        exit(1);
    }

    // find the end of the file, and make a buffer based on the length
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    // allocate a buffer with our newfound knowledge
    buffer = calloc(1, file_size + 1);
    if (!buffer) {
        perror("error: get_file_contents: buffer malloc");
        fprintf(stderr, "error: get_file_contents: buffer malloc problem child: %s\n", filepath);
        fclose(fp);
        exit(1);
    }

    // read file_size bytes from fp into the buffer
    if (fread(buffer, file_size, 1, fp) != 1) {
        perror("error: get_file_contents: fp fread");
        fprintf(stderr, "error: get_file_contents: fp fread problem child: %s\n", filepath);
        fclose(fp);
        free(buffer);
        exit(1);
    }

    // close up the file now that we are done
    fclose(fp);
    return buffer;
}
