#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <vector>
#include <string>

void getDbFile(const std::string &filePath, std::vector<std::string> &dbFile);
