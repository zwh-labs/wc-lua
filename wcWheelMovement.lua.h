#ifndef WC_WHEELMOVEMENT_LUA_H
#define WC_WHEELMOVEMENT_LUA_H

#include <wc/Thread.h>

#include <string.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"


#define LUAWC_WHEELMOVEMENT "wcWheelMovement*"
#define LUAWC_WHEELMOVEMENT_GLOBAL "wcWheelMovement"


static wcWheelMovement * luaWC_toWheelMovement( lua_State * L )
{
	return (wcWheelMovement *) luaL_checkudata( L, 1, LUAWC_WHEELMOVEMENT );
}


static int luaWC_WheelMovement_create( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'create' takes one argument" );

	wcWheelMovement * wm = (wcWheelMovement*) lua_newuserdata( L, sizeof(wcWheelMovement) );

	wcWheelMovement_create( wm, luaL_checkinteger( L, 1 ) );

	luaL_getmetatable( L, LUAWC_WHEELMOVEMENT );
	lua_setmetatable( L, -2 );
	return 1;
}


static int luaWC_WheelMovement_getIndex( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'getIndex' takes one argument" );
	wcWheelMovement * wm = luaWC_toWheelMovement( L );
	unsigned int index = wcWheelMovement_getIndex( wm );
	lua_pushinteger( L, index );
	return 1;
}


static int luaWC_WheelMovement_getIncrements( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'getIncrements' takes one argument" );
	wcWheelMovement * wm = luaWC_toWheelMovement( L );
	int increments = wcWheelMovement_getIncrements( wm );
	lua_pushinteger( L, increments );
	return 1;
}


static const luaL_Reg luaWC_WheelMovement[] =
{
	{"getIndex", luaWC_WheelMovement_getIndex},
	{"getIncrements", luaWC_WheelMovement_getIncrements},
	{NULL, NULL}
};


static const luaL_Reg luaWC_WheelMovement_Global[] =
{
	{"create", luaWC_WheelMovement_create},
	{"getIndex", luaWC_WheelMovement_getIndex},
	{"getIncrements", luaWC_WheelMovement_getIncrements},
	{NULL, NULL}
};


static int luaopen_wcWheelMovement( lua_State * L )
{
	luaL_newmetatable( L, LUAWC_WHEELMOVEMENT );
	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );
	luaL_register( L, NULL, luaWC_WheelMovement );
	luaL_register( L, LUAWC_WHEELMOVEMENT_GLOBAL, luaWC_WheelMovement_Global);
	return 1;
}


#endif
