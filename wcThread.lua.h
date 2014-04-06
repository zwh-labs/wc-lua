#ifndef WC_THREAD_LUA_H
#define WC_THREAD_LUA_H

#include "wcConnection.lua.h"
#include "wcWheelMovement.lua.h"

#include <wc/Thread.h>

#include <string.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"


#define LUAWC_THREAD "wcThread*"
#define LUAWC_THREAD_GLOBAL "wcThread"


static wcThread ** luaWC_topThread( lua_State * L )
{
	return (wcThread **) luaL_checkudata( L, 1, LUAWC_THREAD );
}


static wcThread * luaWC_toThread( lua_State * L )
{
	wcThread ** t = luaWC_topThread( L );
	if( *t == NULL )
		luaL_error( L, "attempt to use an uninitialized thread" );
	return *t;
}


static int luaWC_Thread_start( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'start' takes one argument" );

	wcThread ** t = (wcThread**) lua_newuserdata( L, sizeof(wcThread*) );

	*t = wcThread_start( luaWC_toConnection( L ) );

	if( *t )
	{
		luaL_getmetatable( L, LUAWC_THREAD );
		lua_setmetatable( L, -2 );
	}
	else
	{
		lua_pushnil( L );
	}
	return 1;
}


static int luaWC_Thread_getWheelCount( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'getWheelCount' takes one argument" );
	wcThread * t = luaWC_toThread( L );
	unsigned int ret = wcThread_getWheelCount( t );
	lua_pushinteger( L, ret );
	return 1;
}


static int luaWC_Thread_isRunning( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'isRunning' takes one argument" );
	wcThread * t = luaWC_toThread( L );
	bool ret = wcThread_isRunning( t );
	lua_pushboolean( L, ret );
	return 1;
}


static int luaWC_Thread_retrieveWheelMovement( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 2 )
		return luaL_error( L, "'retrieveWheelMovement' takes two arguments" );
	wcThread * t = luaWC_toThread( L );
	unsigned int index = luaL_checkinteger( L, 2 );

	wcWheelMovement * wm = (wcWheelMovement*) lua_newuserdata( L, sizeof(wcWheelMovement) );
	*wm = wcThread_retrieveWheelMovement( t, index );
	luaL_getmetatable( L, LUAWC_WHEELMOVEMENT );
	lua_setmetatable( L, -2 );
	return 1;
}


static int luaWC_Thread_gc( lua_State * L )
{
	wcThread ** t = luaWC_topThread( L );
	if( *t )
		wcThread_stop( *t );
	*t = NULL;
	return 0;
}


static int luaWC_Thread_stop( lua_State * L )
{
	int argc = lua_gettop( L );
	if( argc != 1 )
		return luaL_error( L, "'stop' takes one argument" );
	wcThread ** t = luaWC_topThread( L );
	bool ret = false;
	if( *t )
		ret = wcThread_stop( *t );
	*t = NULL;
	lua_pushboolean( L, ret );
	return 1;
}


static const luaL_Reg luaWC_Thread[] =
{
	{"stop", luaWC_Thread_stop},
	{"isRunning", luaWC_Thread_isRunning},
	{"getWheelCount", luaWC_Thread_getWheelCount},
	{"retrieveWheelMovement", luaWC_Thread_retrieveWheelMovement},
	{"__gc", luaWC_Thread_gc},
	{NULL, NULL}
};


static const luaL_Reg luaWC_Thread_Global[] =
{
	{"start", luaWC_Thread_start},
	{"stop", luaWC_Thread_stop},
	{"isRunning", luaWC_Thread_isRunning},
	{"getWheelCount", luaWC_Thread_getWheelCount},
	{"retrieveWheelMovement", luaWC_Thread_retrieveWheelMovement},
	{NULL, NULL}
};


static int luaopen_wcThread( lua_State * L )
{
	luaL_newmetatable( L, LUAWC_THREAD );
	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );
	luaL_register( L, NULL, luaWC_Thread );
	luaL_register( L, LUAWC_THREAD_GLOBAL, luaWC_Thread_Global);
	return 1;
}


#endif
