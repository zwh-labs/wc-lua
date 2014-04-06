#include <stdio.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "wcConfiguration.lua.h"
#include "wcConnection.lua.h"
#include "wcThread.lua.h"
#include "wcWheelMovement.lua.h"


int main( int argc, const char ** argv )
{
	if( argc != 2 )
	{
		fprintf( stderr, "Usage: %s <luafile>\n", argv[0] );
		return 1;
	}

	lua_State * L = luaL_newstate();
	luaL_openlibs( L );

	luaopen_wcConfiguration( L );
	luaopen_wcConnection( L );
	luaopen_wcThread( L );
	luaopen_wcWheelMovement( L );

	int status = luaL_dofile( L, argv[1] );
	int result = lua_toboolean( L, -1 );
	if( status )
	{
		const char * msg = ( lua_type( L, -1 ) == LUA_TSTRING ) ? lua_tostring( L, -1 ) : NULL;
		if( msg == NULL )
			msg = "(error object is not a string)";
		fprintf( stderr, "Failed:\n%s\n", msg );
		lua_pop( L, 1 );
	}

	lua_close( L );
	return 0;
}
