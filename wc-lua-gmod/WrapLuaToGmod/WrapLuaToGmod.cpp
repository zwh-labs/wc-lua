#include "WrapLuaToGmod.h"
#include "GarrysMod/Lua/Interface.h"

#include <stdlib.h>
#include <string.h>
#include <alloca.h>


using namespace GarrysMod::Lua;




LUA_API void lua_pushnil( lua_State * state )
{
	return LUA->PushNil();
}


LUA_API void lua_pushinteger( lua_State * state, lua_Integer n )
{
	return LUA->PushNumber( n );
}


LUA_API const char * lua_tolstring( lua_State * state, int index, size_t * len )
{
	return LUA->GetString( index, len );
}


LUA_API void lua_pushstring( lua_State * state, const char * s )
{
	return LUA->PushString( s );
}


LUA_API void lua_pushlstring( lua_State * state, const char * s, size_t len )
{
	return LUA->PushString( s, len );
}


LUA_API int lua_isstring( lua_State * state, int index )
{
	return LUA->IsType( index, Type::STRING ) ? 1 : 0;
}


LUA_API int lua_isuserdata(lua_State * state, int index )
{
	return LUA->IsType( index, Type::USERDATA ) ? 1 : 0;
}


LUA_API void lua_rawget( lua_State * state, int idx )
{
	return LUA->RawGet( idx );
}


LUA_API void lua_rawset( lua_State * state, int idx )
{
	return LUA->RawSet( idx );
}


LUA_API const char * lua_pushfstring( lua_State * state, const char * fmt, ... )
{
	va_list ap;

	va_start( ap, fmt );
	int size = vsnprintf( NULL, 0, fmt, ap) + 1;
	va_end( ap );

	char * buf = (char*) alloca( size );

	va_start( ap, fmt );
	vsnprintf( buf, size, fmt, ap );
	va_end( ap );

	lua_pushstring( state, buf );

	return lua_tolstring( state, -1, NULL );
}


LUA_API void lua_createtable( lua_State * state, int narray, int nrec )
{
	(void)narray;
	(void)nrec;
	return LUA->CreateTable();
}


LUA_API void lua_settable( lua_State * state, int idx )
{
	return LUA->SetTable( idx );
}


LUA_API void lua_remove( lua_State * state, int idx )
{
	return LUA->Remove( idx );
}


LUA_API void lua_insert( lua_State * state, int idx )
{
	return LUA->Insert( idx );
}


LUA_API int lua_next( lua_State * state, int idx )
{
	return LUA->Next( idx );
}


LUA_API void lua_pushcclosure( lua_State * state, lua_CFunction fn, int n )
{
	return LUA->PushCClosure( fn, n );
}


LUA_API int lua_gettop( lua_State * state )
{
	return LUA->Top();
}


LUA_API void * lua_newuserdata( lua_State * state, size_t size )
{
	return LUA->NewUserdata( size );
}


LUA_API void lua_getfield( lua_State * state, int idx, const char * k )
{
	return LUA->GetField( idx, k );
}


LUA_API void lua_setfield( lua_State * state, int idx, const char * k )
{
	return LUA->SetField( idx, k );
}


LUA_API int lua_getmetatable( lua_State * state, int objindex )
{
	return LUA->GetMetaTable( objindex ) ? 1 : 0;
}


LUA_API int lua_setmetatable( lua_State * state, int objindex )
{
	LUA->SetMetaTable( objindex );
	return 1;
}


LUA_API void lua_pushboolean( lua_State * state, int b )
{
	return LUA->PushBool( b!=0 );
}


LUA_API void lua_pushvalue( lua_State * state, int idx )
{
	return LUA->Push( idx );
}


LUA_API int lua_rawequal( lua_State * state, int index1, int index2 )
{
	return LUA->RawEqual( index1, index2 );
}


LUA_API void * lua_touserdata( lua_State * state, int idx )
{
	return LUA->GetUserdata( idx );
}


LUA_API int lua_type( lua_State * state, int idx )
{
	return LUA->GetType( idx );
}


LUA_API const char * lua_typename( lua_State * state, int t )
{
	return LUA->GetTypeName( t );
}




LUALIB_API int luaL_error( lua_State * state, const char *fmt, ... )
{
	va_list ap;

	va_start( ap, fmt );
	int size = vsnprintf( NULL, 0, fmt, ap) + 1;
	va_end( ap );

	char * buf = (char*) alloca( size );

	va_start( ap, fmt );
	vsnprintf( buf, size, fmt, ap );
	va_end( ap );

	LUA->ThrowError( buf );

	return 0;
}


LUALIB_API void * luaL_testudata( lua_State * state, int ud, const char * tname )
{
	void * p = lua_touserdata( state, ud );
	if( p ) /* value is a userdata? */
	{
		if( lua_getmetatable( state, ud ) ) /* does it have a metatable? */
		{
			luaL_getmetatable( state, tname ); /* get correct metatable */
			if( !lua_rawequal( state, -1, -2 ) ) /* not the same? */
				p = NULL; /* value is a userdata with wrong metatable */
			lua_pop( state, 2 ); /* remove both metatables */
			return p;
		}
	}
	return NULL;  /* value is not a userdata with a metatable */
}


static int typeerror( lua_State * state, int narg, const char * tname )
{
	const char * msg = lua_pushfstring( state, "%s expected, got %s", tname, luaL_typename( state, narg ) );
	LUA->ArgError( narg, msg );
	return 0;
}


static void tag_error( lua_State * state, int narg, int tag )
{
	typeerror( state, narg, lua_typename(state, tag) );
}


LUALIB_API lua_Integer luaL_checkinteger( lua_State * state, int narg )
{
	return LUA->CheckNumber( narg );
}


LUALIB_API const char * luaL_checklstring( lua_State * state, int narg, size_t * len )
{
	const char * s = lua_tolstring( state, narg, len );
	if( !s )
		tag_error( state, narg, LUA_TSTRING );
	return s;
}


LUALIB_API void * luaL_checkudata( lua_State * state, int ud, const char * tname )
{
	void * p = luaL_testudata( state, ud, tname );
	if( !p )
		typeerror( state, ud, tname);
	return p;
}


LUALIB_API int luaL_newmetatable( lua_State * state, const char * tname )
{
	LUA->CreateMetaTableType( tname, Type::USERDATA );
	return 1;
}


LUALIB_API const char * luaL_findtable( lua_State * state, int idx,  const char * fname, int szhint )
{
	const char * e;
	if( idx )
		lua_pushvalue( state, idx );
	do {
		e = strchr( fname, '.' );
		if( e == NULL )
			e = fname + strlen( fname );
		lua_pushlstring( state, fname, e - fname );
		lua_rawget( state, -2 );
		if( lua_isnil( state, -1 ) )
		{  /* no such field? */
			lua_pop( state, 1 );  /* remove this nil */
			lua_createtable( state, 0, (*e == '.' ? 1 : szhint) ); /* new table for field */
			lua_pushlstring( state, fname, e - fname );
			lua_pushvalue( state, -2 );
			lua_settable( state, -4 );  /* set new table into field */
		}
		else if( !lua_istable( state, -1 ) )
		{  /* field has a non-table value? */
			lua_pop( state, 2 );  /* remove table and value */
			return fname;  /* return problematic part of the name */
		}
		lua_remove( state, -2 );  /* remove previous table */
		fname = e + 1;
	} while( *e == '.' );
	return NULL;
}


static int libsize( const luaL_Reg * l )
{
	int size = 0;
	for( ; l && l->name; l++ )
		size++;
	return size;
}


LUALIB_API void luaL_pushmodule( lua_State * state, const char * modname, int sizehint )
{
	luaL_findtable( state, LUA_REGISTRYINDEX, "_LOADED", 1 );  /* get _LOADED table */
	lua_getfield( state, -1, modname );  /* get _LOADED[modname] */
	if( !lua_istable( state, -1 ) )
	{  /* not found? */
		lua_pop( state, 1 );  /* remove previous result */
		/* try global variable (and create one if it does not exist) */
		if( luaL_findtable( state, LUA_GLOBALSINDEX, modname, sizehint ) != NULL )
			luaL_error( state, "name conflict for module " LUA_QS, modname );
		lua_pushvalue( state, -1 );
		lua_setfield( state, -3, modname );  /* _LOADED[modname] = new table */
	}
	lua_remove( state, -2 );  /* remove _LOADED table */
}


LUALIB_API void luaL_setfuncs( lua_State * state, const luaL_Reg * l, int nup )
{
	for( ; l->name != NULL; l++ )
	{  /* fill the table with given functions */
		int i;
		for( i = 0; i < nup; i++ )  /* copy upvalues to the top */
			lua_pushvalue( state, -nup );
		lua_pushcclosure( state, l->func, nup );  /* closure with those upvalues */
		lua_setfield( state, -(nup + 2), l->name );
	}
	lua_pop( state, nup );  /* remove upvalues */
}


LUALIB_API void luaL_openlib( lua_State * state, const char * libname, const luaL_Reg * l, int nup )
{
	if( libname )
	{
		luaL_pushmodule( state, libname, libsize(l) );  /* get/create library table */
		lua_insert( state, -(nup+1) );  /* move library table to below upvalues */
	}
	if( l )
		luaL_setfuncs( state, l, nup );
	else
		lua_pop( state, nup );  /* remove upvalues */
}
