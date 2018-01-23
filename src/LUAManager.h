#pragma once

extern "C"
{
	#include <lua50/lua.h>
	#include <lua50/lualib.h>
	#include <lua50/lauxlib.h>
}

#include <string>

class LUAManager
{
	lua_State* pL;
public:
	LUAManager(char *fitxer);
	~LUAManager(void);
	// Obtenir cadenes de variables soltes
	std::string getCadena(char *camp);
	// el mateix però amb taules
	std::string getTaulaCadena(char *camp, char *element);
	// Obtenir números enters de variables
	int getNumeroint(char *camp);
	// El mateix però amb taules
	int getTaulaNumeroint(char *camp, char *element);
	// Obtenir floats
	float getNumerofloat(char *camp);
	// El mateix però amb taules
	float getTaulaNumerofloat(char *camp, char *element);
	
};
