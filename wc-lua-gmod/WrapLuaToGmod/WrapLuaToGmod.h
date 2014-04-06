#ifndef WRAPLUATOGMOD_H
#define WRAPLUATOGMOD_H

#ifdef lconfig_h
	#error "Include WrapLua2Gmod.h before including any lua headers!"
#endif

// we will wrap all original lua functions to the gmod interface - pretend to be the lua library itself
#define LUA_CORE
#define LUA_LIB

#include "luaconf.h"

// re-define all lua functions that used to be visible to library users as internal functions
#undef LUA_API
#define LUA_API LUAI_FUNC

#include "lua.h"
#include "lauxlib.h"

#undef lua_pop
#define lua_pop(L,n) (L->luabase->Pop(n))

#ifndef luaL_register
#define luaL_register(L,n,l) (luaL_openlib(L,(n),(l),0))
#endif

#endif
