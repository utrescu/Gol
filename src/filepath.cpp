#include "filepath.h"
#include <string.h>

const std::string getFullFileName(const char *nom)
{
    std::string buf(PATH.c_str());
    buf.append("/");
    buf.append(nom);
    printf("Open %s -> %s\n", nom, buf.c_str());
    return buf;
}

const std::string getFullFileName(std::string nom)
{
    std::string buf = PATH;
    buf.append("/");
    buf.append(nom);
    return buf;
}