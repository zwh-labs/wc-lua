#ifndef WC_CONNECTION_LUA_H
#define WC_CONNECTION_LUA_H

#include "wcConfiguration.lua.h"

#include <wc/Connection.h>
#include <wc/Connection_Configuration.h>

#include <string.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"


#define LUAWC_CONNECTION "wcConnection*"
#define LUAWC_CONNECTION_GLOBAL "wcConnection"


static wcConnection ** luaWC_topConnection( lua_State * L )
{
	return (wcConnection **) luaL_checkudata( L, 1, LUAWC_CONNECTION );
}


static wcConnection * luaWC_toConnection( lua_State * L )
{
	wcConnection ** c = luaWC_topConnection( L );
	if( *c == NULL )
		luaL_error( L, "attempt to use an uninitialized connection" );
	return *c;
}


static int luaWC_Connection_open( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'open' takes one argument" );

	wcConnection ** c = (wcConnection**) lua_newuserdata( L, sizeof(wcConnection*) );

	if( lua_isstring( L, 1 ) )
		*c = wcConnection_open( luaL_checkstring( L, 1 ) );
	else if( lua_isuserdata( L, 1 ) )
		*c = wcConnection_openFromConfiguration( luaWC_toConfiguration( L ) );
	else
		return luaL_error( L, "'open' takes either a string or a configuration" );

	if( *c )
	{
		luaL_getmetatable( L, LUAWC_CONNECTION );
		lua_setmetatable( L, -2 );
	}
	else
	{
		lua_pushnil( L );
	}
	return 1;
}


static int luaWC_Connection_gc( lua_State * L )
{
	wcConnection ** c = luaWC_topConnection( L );
	if( *c )
		wcConnection_close( *c );
	*c = NULL;
	return 0;
}


static int luaWC_Connection_close( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'close' takes one argument" );
	wcConnection ** c = luaWC_topConnection( L );
	bool ret = false;
	if( *c )
		ret = wcConnection_close( *c );
	*c = NULL;
	lua_pushboolean( L, ret );
	return 1;
}


static const luaL_Reg luaWC_Connection[] =
{
	{"close", luaWC_Connection_close},
	{"__gc", luaWC_Connection_gc},
	{NULL, NULL}
};


static const luaL_Reg luaWC_Connection_Global[] =
{
	{"open", luaWC_Connection_open},
	{"close", luaWC_Connection_close},
	{NULL, NULL}
};


static int luaopen_wcConnection( lua_State * L )
{
	luaL_newmetatable( L, LUAWC_CONNECTION );
	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );
	luaL_register( L, NULL, luaWC_Connection );
	luaL_register( L, LUAWC_CONNECTION_GLOBAL, luaWC_Connection_Global);
	return 1;
}


#endif
