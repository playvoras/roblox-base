#include "closure.hpp"

class bytecode_encoder_t : public Luau::BytecodeEncoder
{
	inline void encode(uint32_t* data, size_t count) override
	{
		for (auto i = 0u; i < count;)
		{
			auto& opcode = *reinterpret_cast<uint8_t*>(data + i);
			i += Luau::getOpLength(LuauOpcode(opcode));
			opcode *= 227;
		}
	}
};

int loadstring(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TSTRING);

	static auto encoder = bytecode_encoder_t();
	auto bytecode = Luau::compile(lua_tostring(L, 1), {}, {}, &encoder);
	if (luau_load(L, luaL_optstring(L, 2, "@"), bytecode.c_str(), bytecode.size(), 0))
	{
		lua_pushnil(L);
		lua_insert(L, -2);
		return 2;
	}
	return 1;
}

void closure::initialize(lua_State* L)
{
	static const luaL_Reg entries[] = {
		{"loadstring", loadstring},
		{nullptr, nullptr}
	};

	lua_pushvalue(L, LUA_GLOBALSINDEX);
	luaL_register(L, nullptr, entries);

	lua_pop(L, 1);

	script_scheduler->add_to_queue(R"(
		getgenv().iscclosure = function(func)
			assert(type(func) == "function", "Expected </iscclosure.func> to be </lua.function>[ENV], got </lua.nop>[EOF]")
			return debug.info(func, "s") == "[C]"
		end

		getgenv().islclosure = function(func)
			assert(type(func) == "function", "Expected </iscclosure.func> to be </lua.function>[ENV], got </lua.nop>[EOF]")
			return original_debug.info(func, "s") ~= "[C]"
		end

		getgenv().newcclosure = function(func)
			if iscclosure(func) then
				return func
			end

			return coroutine.wrap(function(...)
				local args = {...}

				while true do
					args = { coroutine.yield(func(unpack(args))) }
				end
			end)
		end

		getgenv().newlclosure = function(func)
			return function(...)
				return func(...)
			end
		end

		getgenv().clonefunction = function(func)
			return function(...) return func(...) end
		end

		getgenv().isexecutorclosure = function(func)
			if func == print then
				return false
			end

			if not table.find(getrenv(), func) then
				return true
			else
				return false
			end
		end
	)");
}