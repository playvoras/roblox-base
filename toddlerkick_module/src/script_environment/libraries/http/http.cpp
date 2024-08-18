#include "http.hpp"

void http::initialize(lua_State* L)
{
	// ...
}

auto http::http_get( lua_State* L ) -> int
{
    luaL_stackcheck( L, 2, 1, luaL_checktype( L, 1, LUA_TSTRING ); luaL_checktype( L, 2, LUA_TSTRING ););

    const char* url = lua_tostring( L, 1 );
    if ( !url )
    {
        luaL_error( L, "Invalid URL provided" );
        return 0;
    }

    std::string content;
    try
    {
        content = http::download_string( url );
    }
    catch ( const std::exception& e )
    {
        luaL_error( L, "Failed to download content from URL: %s", e.what( ) );
        return 0;
    }

    lua_pushstring( L, content.c_str( ) );
    return 1;
}
