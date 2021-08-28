#include "util.h"

void getDbFile(const std::string &filePath, std::vector<std::string> &dbFile)
{

    DIR *srcDir = opendir(filePath.c_str());
    if (NULL == srcDir)
    {
        printf("open dir %s error, pls check it.\n", filePath.c_str());
        exit(EXIT_FAILURE);
    }

    struct dirent *tmp = NULL;
    while (1)
    {
        tmp = readdir(srcDir);
        if (NULL == tmp)
            break;
        if (strcmp(".", tmp->d_name) == 0 ||
            strcmp("..", tmp->d_name) == 0)
            continue;
        if (tmp->d_type == DT_REG)
        {
            dbFile.push_back(tmp->d_name);
        }
    }
    closedir(srcDir);
}