#ifndef WC_CONFIGURATION_LUA_H
#define WC_CONFIGURATION_LUA_H

#include <wc/Configuration.h>
#include <wc/Configuration_stdio.h>

#include <string.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"


#define LUAWC_CONFIGURATION "wcConfiguration*"
#define LUAWC_CONFIGURATION_GLOBAL "wcConfiguration"


static wcConfiguration ** luaWC_topConfiguration( lua_State * L )
{
	return (wcConfiguration **) luaL_checkudata( L, 1, LUAWC_CONFIGURATION );
}


static wcConfiguration * luaWC_toConfiguration (lua_State * L)
{
	wcConfiguration ** c = luaWC_topConfiguration( L );
	if( *c == NULL )
		luaL_error( L, "attempt to use an uninitialized configuration" );
	return *c;
}


static int luaWC_Configuration_toString( lua_State * L )
{
	wcConfiguration * c = *luaWC_topConfiguration(L);

	int size = wcConfiguration_snprint( NULL, 0, c );
	if( size < 0 )
		return luaL_error( L, "could not format output string" );
	size += 1;	// + 1 for terminating '\0'
	char * buf = (char*) malloc( size );
	if( !buf )
		return luaL_error( L, "could not allocate memory for output string" );
	wcConfiguration_snprint( buf, size, c );

	lua_pushstring( L, buf );
	free( buf );
	return 1;
}


static int luaWC_Configuration_new( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 0 )
		return luaL_error( L, "'new' takes no arguments" );

	wcConfiguration ** c = (wcConfiguration**) lua_newuserdata( L, sizeof(wcConfiguration*) );
	*c = wcConfiguration_new();
	luaL_getmetatable( L, LUAWC_CONFIGURATION );
	lua_setmetatable( L, -2 );
	return 1;
}


static int luaWC_Configuration_gc( lua_State * L )
{
	wcConfiguration ** c = luaWC_topConfiguration( L );
	if( *c )
		wcConfiguration_delete( *c );
	*c = NULL;
	return 0;
}


static int luaWC_Configuration_delete( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'delete' takes one argument" );
	wcConfiguration ** c = luaWC_topConfiguration( L );
	bool ret = false;
	if( *c )
		ret = wcConfiguration_delete( *c );
	*c = NULL;
	lua_pushboolean( L, ret );
	return 1;
}


static int luaWC_Configuration_getDevicePath( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'getDevicePath' takes one argument" );
	wcConfiguration * c = luaWC_toConfiguration( L );
	const char * ret = wcConfiguration_getDevicePath( c );
	lua_pushstring( L, ret );
	return 1;
}


static int luaWC_Configuration_setDevicePath( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 2 )
		return luaL_error( L, "'setDevicePath' takes two arguments" );
	wcConfiguration * c = luaWC_toConfiguration( L );
	const char * devicePath = luaL_checkstring( L, 2 );
	wcConfiguration_setDevicePath( c, devicePath );
	return 0;
}


static int luaWC_Configuration_getWheelCount( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'getWheelCount' takes one argument" );
	wcConfiguration * c = luaWC_toConfiguration( L );
	int wheelCount = wcConfiguration_getWheelCount( c );
	lua_pushinteger( L, wheelCount );
	return 1;
}


static int luaWC_Configuration_getWheelIncrementsPerTurn( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 2 )
		return luaL_error( L, "'getWheelIncrementsPerTurn' takes two arguments" );
	wcConfiguration * c = luaWC_toConfiguration( L );
	int wheelIndex = luaL_checkinteger( L, 2 );
	int wheelIncrementsPerTurn = wcConfiguration_getWheelIncrementsPerTurn( c, wheelIndex );
	lua_pushinteger( L, wheelIncrementsPerTurn );
	return 1;
}


static int luaWC_Configuration_setWheel( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 3 )
		return luaL_error( L, "'setWheel' takes 3 arguments" );
	wcConfiguration * c = luaWC_toConfiguration( L );
	int wheelIndex = luaL_checkinteger( L, 2 );
	int wheelIncrementsPerTurn = luaL_checkinteger( L, 3 );
	bool ret = wcConfiguration_setWheel( c, wheelIndex, wheelIncrementsPerTurn );
	lua_pushboolean( L, ret );
	return 1;
}


static const luaL_Reg luaWC_Configuration[] =
{
	{"delete", luaWC_Configuration_delete},
	{"getDevicePath", luaWC_Configuration_getDevicePath},
	{"setDevicePath", luaWC_Configuration_setDevicePath},
	{"getWheelCount", luaWC_Configuration_getWheelCount},
	{"getWheelIncrementsPerTurn", luaWC_Configuration_getWheelIncrementsPerTurn},
	{"setWheel", luaWC_Configuration_setWheel},
	{"__gc", luaWC_Configuration_gc},
	{"__tostring", luaWC_Configuration_toString},
	{NULL, NULL}
};


static const luaL_Reg luaWC_Configuration_Global[] =
{
	{"new", luaWC_Configuration_new},
	{"delete", luaWC_Configuration_delete},
	{"getDevicePath", luaWC_Configuration_getDevicePath},
	{"setDevicePath", luaWC_Configuration_setDevicePath},
	{"getWheelCount", luaWC_Configuration_getWheelCount},
	{"getWheelIncrementsPerTurn", luaWC_Configuration_getWheelIncrementsPerTurn},
	{"setWheel", luaWC_Configuration_setWheel},
	{NULL, NULL}
};


static int luaopen_wcConfiguration( lua_State * L )
{
	luaL_newmetatable( L, LUAWC_CONFIGURATION );
	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );
	luaL_register( L, NULL, luaWC_Configuration );
	luaL_register( L, LUAWC_CONFIGURATION_GLOBAL, luaWC_Configuration_Global);
	return 1;
}


#endif
