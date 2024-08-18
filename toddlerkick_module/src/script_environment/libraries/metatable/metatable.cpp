#include "metatable.hpp"

int get_raw_metatable(lua_State* L)
{
	luaL_stackcheck(L, 1, 1);

	luaL_checktypes(L, 1, LUA_TTABLE, LUA_TUSERDATA);
	lua_getmetatable(L, 1);
	return 1;
}

int set_raw_metatable(lua_State* L)
{
	luaL_stackcheck(L, 2, 2);

	luaL_checktypes(L, 1, LUA_TTABLE, LUA_TUSERDATA);
	luaL_checktypes(L, 2, LUA_TTABLE, LUA_TNIL);

	lua_settop(L, 2);
	lua_pushboolean(L, lua_setmetatable(L, 1));
	return 1;
}

int get_namecall_method(lua_State* L)
{
	lua_pushstring(L, L->namecall ? getstr(L->namecall) : nullptr);
	return 1;
}

int set_namecall_method(lua_State* L)
{
	luaL_stackcheck(L, 1, 1);

	luaL_checktype(L, 1, LUA_TSTRING);
	L->namecall = tsvalue(luaA_toobject(L, 1));
	return 0;
}

int is_readonly(lua_State* L)
{
	luaL_stackcheck(L, 1, 1);

	luaL_checktypes(L, 1, LUA_TTABLE, LUA_TUSERDATA);
	lua_pushboolean(L, lua_getreadonly(L, 1));
	return 1;
}

int set_readonly(lua_State* L)
{
	luaL_stackcheck(L, 2, 2);

	luaL_checktypes(L, 1, LUA_TTABLE, LUA_TUSERDATA);
	luaL_checktype(L, 2, LUA_TBOOLEAN);

	lua_setreadonly(L, 1, lua_toboolean(L, 2));
	return 0;
}

void metatable::initialize(lua_State* L)
{
	static const luaL_Reg entries[] = {
		{"getrawmetatable", get_raw_metatable},
		{"setrawmetatable", set_raw_metatable},
		{"getnamecallmethod", get_namecall_method},
		{"setnamecallmethod", set_namecall_method},
		{"isreadonly", is_readonly},
		{"setreadonly", set_readonly},
		{nullptr, nullptr}
	};

	lua_pushvalue(L, LUA_GLOBALSINDEX);
	luaL_register(L, nullptr, entries);

	lua_pop(L, 1);
}