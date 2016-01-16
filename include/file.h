#ifndef _FILE_H_
#define _FILE_H_

/**
 * get_file_contents
 *
 * get the contents of a file on disk given the filepath
 *
 * if any errors occur during the reading of the file on disk, including
 * opening the file, allocating memory, and actually reading the file, a
 * detailed error message is printed to stderr and NULL is returned
 *
 * arguments:
 *  const char *filepath - the relative filepath of the file to return the
 *      contents of
 *
 * returns:
 *  a char * allocated with calloc to exactly the size of the file passed,
 *  containing the contents of the file, or NULL on error (see above)
 */
char *get_file_contents (const char *filepath);

#endif // _FILE_H_
