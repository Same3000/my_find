#include "simple_ls.h"

static int aux_my_ls(DIR *d, char *path, struct files *list)
{
    struct stat s;
    struct dirent *entry = readdir(d);
    if (entry == NULL)
        return -1;
    /*entry = readdir(d);
    if (entry == NULL)
        return -1;
    entry = readdir(d);*/
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
        //printf("%s\n", pathtmp);
		list = files_append(list, entry->d_name, pathtmp);
        int tmp = stat(pathtmp, &s);
        if (tmp == -1)
            return -1;
        if (S_ISDIR(s.st_mode))
        {
            DIR *direct = opendir(pathtmp);
            if (aux_my_ls(direct, pathtmp, list) == -1)
                return -1;
            closedir(direct);
        }
        entry = readdir(d);
    }
    return 0;
}

int my_ls(int argc, char *argv[], int start, struct files *list)
{
    DIR *di = NULL;
    if (argc > 1 && argv[start][0] != '-')
    {
        for (int i = start; i < argc && argv[i][0] != '-'; i++)
        {
            //printf("%s\n", argv[i]);
			list = files_append(list, argv[i], argv[i]);
			di = opendir(argv[i]);
            if (di == NULL)
                return -1;
            aux_my_ls(di, argv[i], list);
           	closedir(di);
        }
    }
    else
    {
		list = files_append(list, ".", ".");
        //printf(".\n");
        di = opendir(".");
        if (aux_my_ls(di, ".", list) == -1)
        {
            closedir(di);
            return 0;
        }
		closedir(di);
    }

    if (di == NULL)
    {
        fprintf(stderr, "simple_ls: %s: No such file or directory\n",
                argv[argc - 1]);
        return 1;
    }
    return 0;
}
