#ifndef SIMPLE_LS_H
#define SIMPLE_LS_H

#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "file.h"

int my_ls(int argc, char *argv[], int *start, struct files *list);

#endif /* SIMPLE_LS_H */
