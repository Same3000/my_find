#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct file
{
	int boolval;
	char *path;
	char *name;
};

struct files
{
	size_t size;
    size_t capacity;
	struct file **data;
};
struct file *create_file(char *name, char *path);
struct files *files_init(size_t n);
struct files *files_resize(struct files *l, size_t n);
struct files *files_append(struct files *l, char *name, char *path);
void files_false(struct files *l, size_t index);
void free_files(struct files *destroy);


#endif /* FILE_H */
