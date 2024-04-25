#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>

int my_ls(DIR *d)
{
    struct dirent *entry = readdir(d);
    if (entry == NULL)
        return -1;
    while (entry != NULL)
    {
        if (entry->d_name[0] != '.')
            printf("%s\n", entry->d_name);
        entry = readdir(d);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    DIR *di = NULL;
    if (argc > 1)
        di = opendir(argv[argc - 1]);
    else
        di = opendir(".");
    if (di == NULL)
    {
        fprintf(stderr, "simple_ls: %s: No such file or directory\n",
                argv[argc - 1]);
        return 1;
    }
    if (my_ls(di) == -1)
    {
        closedir(di);
        return 0;
    }
    closedir(di);
    return 0;
}
