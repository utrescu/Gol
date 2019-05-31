#pragma once

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <string>
#include "filepath.h"

class LUAManager
{
	lua_State *pL;

public:
	LUAManager(const char *fitxer);
	~LUAManager(void);
	// Obtenir cadenes de variables soltes
	std::string getCadena(const char *camp);
	// el mateix però amb taules
	std::string getTaulaCadena(const char *camp, const char *element);
	// Obtenir números enters de variables
	int getNumeroint(const char *camp);
	// El mateix però amb taules
	int getTaulaNumeroint(const char *camp, const char *element);
	// Obtenir floats
	float getNumerofloat(const char *camp);
	// El mateix però amb taules
	float getTaulaNumerofloat(const char *camp, const char *element);
};
