#include "simple_ls.h"

static int aux_my_ls(DIR *d, char *path, struct files *list)
{
    struct stat s;
    struct dirent *entry = readdir(d);
    if (entry == NULL)
        return 1;
    char pathtmp[150];
    while (entry != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            entry = readdir(d);
            continue;
        }
        strcpy(pathtmp, path);
        if (pathtmp[strlen(path) - 1] != '/')
            strcat(pathtmp, "/");
        strcat(pathtmp, entry->d_name);
        // printf("%s\n", pathtmp);
        if (list->d == 0)
            list = files_append(list, entry->d_name, pathtmp);
        int tmp = stat(pathtmp, &s);
        if (tmp == -1)
            return -1;
        if (S_ISDIR(s.st_mode))
        {
            DIR *direct = opendir(pathtmp);
            if (aux_my_ls(direct, pathtmp, list) == 1)
                return 1;
            closedir(direct);
        }
        if (list->d == 1)
            list = files_append(list, entry->d_name, pathtmp);
        entry = readdir(d);
    }
    return 0;
}
static void aux(char *arg, struct files *list, int val)
{
    if (list->d == val)
        list = files_append(list, arg, arg);
}
int my_ls(int argc, char *argv[], int *start, struct files *list)
{
    struct stat s;
    DIR *di = NULL;
    int i = 0;
    if (argc > 1 && argv[*start][0] != '-')
    {
        while (*start < argc && argv[*start][0] != '-')
        {
            i = *start;
            (*start)++;
            di = opendir(argv[i]);
            aux(argv[i], list, 0);
            if (di == NULL)
            {
                aux(argv[i], list, 1);
                if (stat(argv[i], &s) == -1)
                {
                    list->data[list->size - 1]->boolval = -1;
                    return 1;
                }
                continue;
            }
            aux_my_ls(di, argv[i], list);
            aux(argv[i], list, 1);
            closedir(di);
        }
    }
    else
    {
        aux(".", list, 0);
        di = opendir(".");
        if (aux_my_ls(di, ".", list) == 1)
        {
            closedir(di);
            errx(1, "Invalid");
        }
        aux(".", list, 1);
        closedir(di);
    }
    return 0;
}
