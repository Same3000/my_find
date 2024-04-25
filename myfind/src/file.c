#include "file.h"

void free_files(struct files *destroy)
{
    for (size_t i = 0; i < destroy->size; i++)
    {
        free(destroy->data[i]->path);
        free(destroy->data[i]->name);
        free(destroy->data[i]);
    }
    free(destroy->data);
    free(destroy);
}

struct file *create_file(char *name, char *path)
{
    struct file *res = malloc(sizeof(struct file));
    res->boolval = 1;
    res->path = malloc(strlen(path) + 1);
    strcpy(res->path, path);
    res->name = malloc(strlen(name) + 1);
    strcpy(res->name, name);
    return res;
}

struct files *files_init(size_t n, int depth)
{
    struct files *res = malloc(sizeof(struct files));
    res->d = depth;
    res->size = 0;
    res->capacity = n;
    res->data = malloc(sizeof(struct file *) * n);
    for (size_t i = 0; i < n; i++)
        res->data[i] = NULL;
    return res;
}
struct files *files_resize(struct files *l, size_t n)
{
    if (n != l->capacity)
    {
        l->data = realloc(l->data, n * sizeof(struct file *));
        if (l == NULL)
            return NULL;
        l->capacity = n;
    }
    if (l->size > n)
        l->size = n;
    return l;
}

struct files *files_append(struct files *l, char *name, char *path)
{
    if (l == NULL)
        return NULL;
    if (l->size == l->capacity)
    {
        l = files_resize(l, l->capacity * 2);
        if (l == NULL)
            return NULL;
    }
    struct file *elt = create_file(name, path);
    l->data[l->size] = elt;
    l->size++;
    return l;
}

void files_false(struct files *l, size_t index)
{
    if (index >= l->size)
        return;
    l->data[index]->boolval = 0;
}
