#include "LUAManager.h"

LUAManager::LUAManager(const char *fitxer)
{
	int error;

	pL = luaL_newstate();
	luaopen_base(pL);
	luaopen_string(pL);
	luaopen_table(pL);
	luaopen_math(pL);
	luaopen_io(pL);
	// Carrega i compila el fitxer LUA
	if ((error = luaL_dofile(pL, fitxer)) != 0)
	{
		// std::cout << "Error " << error << ": Problemes amb el fitxer de LUA" << endl;
	}
}

LUAManager::~LUAManager(void)
{
	lua_close(pL);
}

std::string LUAManager::getCadena(const char *camp)
{
	// Mirar que la pila est�en blanc
	lua_settop(pL, 0);
	// Carregar la variable a la pila
	lua_getglobal(pL, camp);
	// Comprovar que coincideix el tipus
	if (!lua_isstring(pL, 1))
	{
		//	cout << "ERROR: Tipus inv�id\n" << endl;
		return "";
	}
	// Obtenir el valor de la cadena
	std::string val = (const char *)lua_tostring(pL, 1);
	// Treure el valor de la pila
	lua_pop(pL, 1);
	// Tornar-lo o peta tot al alliberar LUA
	return val;
}

std::string LUAManager::getTaulaCadena(const char *camp, const char *element)
{
	// Buidar la pila
	lua_settop(pL, 0);
	// Posar la variable demanada a la pila
	lua_getglobal(pL, camp);
	// Comprovar que el tipus � correcte  (comencen per 1 )
	if (!lua_istable(pL, 1))
	{
		// cout << "ERROR: Tipus inv�id\n" << endl;
		return "";
	}
	else
	{
		// Entrar l'�dex de l'element
		lua_pushstring(pL, element);
		// Canviar el nom pel valor de la taula
		lua_gettable(pL, -2);
		if (!lua_isstring(pL, -1))
		{
			return "";
		}
		// Obtenir el valor de la taula
		std::string val = lua_tostring(pL, -1);
		// treure el valor de la pila
		lua_pop(pL, 1);
		// Retornar el valor
		return val;
	}
	return "";
}

int LUAManager::getNumeroint(const char *camp)
{
	// Buidar la pila
	lua_settop(pL, 0);
	// Posar la variable demanada a la pila
	lua_getglobal(pL, camp);
	// Comprovar que el tipus és correcte  (comencen per 1 )
	if (!lua_isnumber(pL, 1))
	{
		// cout << "ERROR: Tipus invàlid\n" << endl;
		return -1;
	}

	int val = (int)lua_tonumber(pL, 1);
	// treure el valor de la pila
	lua_pop(pL, 1);

	return val;
}

float LUAManager::getNumerofloat(const char *camp)
{
	// Buidar la pila
	lua_settop(pL, 0);
	// Posar la variable demanada a la pila
	lua_getglobal(pL, camp);
	// Comprovar que el tipus és correcte  (comencen per 1 )
	if (!lua_isnumber(pL, 1))
	{
		// cout << "ERROR: Tipus invàlid\n" << endl;
		return -1;
	}

	float val = (float)lua_tonumber(pL, 1);
	// treure el valor de la pila
	lua_pop(pL, 1);

	return val;
}

int LUAManager::getTaulaNumeroint(const char *camp, const char *element)
{
	// Buidar la pila
	lua_settop(pL, 0);
	// Posar la variable demanada a la pila
	lua_getglobal(pL, camp);
	// Comprovar que el tipus és correcte  (comencen per 1 )
	if (!lua_istable(pL, 1))
	{
		// cout << "ERROR: Tipus invàlid\n" << endl;
		return -1;
	}
	else
	{
		// Entrar l'índex de l'element
		lua_pushstring(pL, element);
		// Canviar el nom pel valor de la taula
		lua_gettable(pL, -2);
		if (!lua_isnumber(pL, -1))
		{
			return -1;
		}
		// Obtenir el valor de la taula
		int val = (int)lua_tonumber(pL, -1);
		// treure el valor de la pila
		lua_pop(pL, 1);
		// Retornar el valor
		return val;
	}
	return -2;
}

float LUAManager::getTaulaNumerofloat(const char *camp, const char *element)
{
	// Buidar la pila
	lua_settop(pL, 0);
	// Posar la variable demanada a la pila
	lua_getglobal(pL, camp);
	// Comprovar que el tipus és correcte  (comencen per 1 )
	if (!lua_istable(pL, 1))
	{
		// cout << "ERROR: Tipus invàlid\n" << endl;
		return -1;
	}
	else
	{
		// Entrar l'index de l'element
		lua_pushstring(pL, element);
		// Canviar el nom pel valor de la taula
		lua_gettable(pL, -2);
		if (!lua_isnumber(pL, -1))
		{
			return -1;
		}
		// Obtenir el valor de la taula
		float val = (float)lua_tonumber(pL, -1);
		// treure el valor de la pila
		lua_pop(pL, 1);
		// Retornar el valor
		return val;
	}
	return -2;
}
