#pragma once

#include <string>
#ifndef DATADIR
const std::string PATH = ".";
#else
const std::string PATH = DATADIR;

#endif

const std::string getFullFileName(const char *nom);
const std::string getFullFileName(std::string nom);
