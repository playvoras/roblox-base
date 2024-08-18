#include "file_system.hpp"

std::filesystem::path get_workspace()
{
	static const auto workspace_path = std::filesystem::current_path() / "workspace";
	std::filesystem::create_directory(workspace_path); // If the path already exists, the call is ignored.

	return workspace_path;
}

std::filesystem::path sanitize(std::filesystem::path path)
{
	static const auto workspace_path = get_workspace();
	const auto sanitized_path = std::filesystem::weakly_canonical(workspace_path / path);

	bool contains_blacklisted_ext = false;
	const char* blacklisted_exts[] = { ".exe", ".scr", ".bat", ".com", ".csh",".msi", ".vb", ".vbs", ".vbe", ".ws", ".wsf", ".wsh", ".ps1", ".py" };

	for (const auto blacklisted_ext : blacklisted_exts)
	{
		if (sanitized_path.string().contains(blacklisted_ext))
			contains_blacklisted_ext = true;
	}

	return (!sanitized_path.string().starts_with(workspace_path.string()) || contains_blacklisted_ext) ? std::filesystem::path() : sanitized_path;
}

int read_file(lua_State* L)
{
	luaL_stackcheck(L, 1, 1, luaL_checktype(L, 1, LUA_TSTRING););

	const auto file_path = sanitize(lua_tostring(L, 1));
	if (!std::filesystem::exists(file_path))
	{
		luaL_error(L, "file \"%s\" does not exist", file_path);
		return 0;
	}

	std::ifstream file_stream(file_path, std::ios::binary);
	if (!file_stream.is_open())
	{
		luaL_error(L, "unable to open \"%s\"", file_path);
		return 0;
	}

	const auto content = std::string(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>());
	file_stream.close();

	lua_pushlstring(L, content.c_str(), content.size());
	return 1;
}

int list_files(lua_State* L)
{
	luaL_stackcheck(L, 1, 1, luaL_checktype(L, 1, LUA_TSTRING););

	const auto folder_path = sanitize(lua_tostring(L, 1));
	if (!std::filesystem::exists(folder_path))
		luaL_error(L, "folder \"%s\" does not exist", folder_path);

	lua_newtable(L);

	int n = 0;
	for (auto& entry : std::filesystem::directory_iterator(folder_path))
	{
		lua_pushstring(L, entry.path().string().c_str());
		lua_rawseti(L, -2, ++n);
	}

	return 1;
}

int write_file(lua_State* L)
{
	luaL_stackcheck(L, 2, 2, luaL_checktype(L, 1, LUA_TSTRING););
	luaL_checktype(L, 2, LUA_TSTRING);

	const auto file_path = sanitize(lua_tostring(L, 1));
	if (!std::filesystem::exists(file_path))
	{
		luaL_error(L, "file \"%s\" does not exist", file_path);
		return 0;
	}

	std::ofstream file_stream(file_path, std::ios::binary);
	if (!file_stream.is_open())
	{
		luaL_error(L, "unable to open \"%s\"", file_path);
		return 0;
	}

	std::string content = lua_tostring(L, 2);
	file_stream.write(content.c_str(), content.size());

	return 0;
}

int make_folder(lua_State* L)
{
	luaL_stackcheck(L, 1, 1, luaL_checktype(L, 1, LUA_TSTRING););
	std::filesystem::create_directories(sanitize(lua_tostring(L, 1)));
	return 0;
}

int append_file(lua_State* L)
{
	luaL_stackcheck(L, 2, 2, luaL_checktype(L, 1, LUA_TSTRING););
	luaL_checktype(L, 2, LUA_TSTRING);

	const auto file_path = sanitize(lua_tostring(L, 1));
	if (!std::filesystem::exists(file_path))
	{
		luaL_error(L, "file \"%s\" does not exist", file_path);
		return 0;
	}

	std::ofstream file_stream(file_path, std::ios::binary | std::ios::app);
	if (!file_stream.is_open())
	{
		luaL_error(L, "unable to open \"%s\"", file_path);
		return 0;
	}

	std::string content = lua_tostring(L, 2);
	file_stream.write(content.c_str(), content.size());

	return 0;
}

int is_file(lua_State* L)
{
	luaL_stackcheck(L, 1, 1, luaL_checktype(L, 1, LUA_TSTRING););
	lua_pushboolean(L, std::filesystem::is_regular_file(sanitize(lua_tostring(L, 1))));
	return 1;
}

int is_folder(lua_State* L)
{
	luaL_stackcheck(L, 1, 1, luaL_checktype(L, 1, LUA_TSTRING););
	lua_pushboolean(L, std::filesystem::is_directory(sanitize(lua_tostring(L, 1))));
	return 1;
}

int del_file(lua_State* L)
{
	luaL_stackcheck(L, 1, 1, luaL_checktype(L, 1, LUA_TSTRING););

	const auto file_path = sanitize(lua_tostring(L, 1));
	if (!std::filesystem::exists(file_path))
	{
		luaL_error(L, "file \"%s\" does not exist", file_path);
		return 0;
	}

	if (!std::filesystem::remove(sanitize(file_path)))
		luaL_error(L, "failed to delete \"%s\"", file_path);

	return 0;
}

int del_folder(lua_State* L)
{
	luaL_stackcheck(L, 1, 1, luaL_checktype(L, 1, LUA_TSTRING););

	const auto folder_path = sanitize(lua_tostring(L, 1));
	if (!std::filesystem::exists(folder_path))
	{
		luaL_error(L, "folder \"%s\" does not exist", folder_path);
		return 0;
	}

	if (!std::filesystem::remove_all(sanitize(folder_path)))
		luaL_error(L, "failed to delete \"%s\"", folder_path);

	return 0;
}

int load_file(lua_State* L)
{
	luaL_stackcheck(L, 1, 1, luaL_checktype(L, 1, LUA_TSTRING););

	const auto file_path = sanitize(lua_tostring(L, 1));
	if (!std::filesystem::exists(file_path))
	{
		luaL_error(L, "file \"%s\" does not exist", file_path);
		return 0;
	}

	std::ifstream file_stream(file_path, std::ios::binary);
	if (!file_stream.is_open())
	{
		luaL_error(L, "unable to open \"%s\"", file_path);
		return 0;
	}

	const auto content = std::string(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>());
	file_stream.close();

	script_scheduler->add_to_queue(content.c_str());
	return 0;
}

void file_system::initialize(lua_State* L)
{
	static const luaL_Reg entries[] = {
		{"readfile", read_file},
		{"writefile", write_file},
		{"appendfile", append_file},
		{"loadfile", load_file},
		{"listfiles", list_files},
		{"isfolder", is_folder},
		{"isfile", is_file},
		{"makefolder", make_folder},
		{"delfolder", del_folder},
		{"delfile", del_file},
		{nullptr, nullptr}
	};

	lua_pushvalue(L, LUA_GLOBALSINDEX);
	luaL_register(L, nullptr, entries);

	lua_pop(L, 1);
}