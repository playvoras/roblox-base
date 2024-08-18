#include "other.hpp"

int getgenv(lua_State* L)
{
	luaL_stackcheck(L, 0, 0);
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	return 1;
}

int getreg(lua_State* L)
{
	luaL_stackcheck(L, 0, 0);
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	return 1;
}

int identify_executor(lua_State* L)
{
	lua_pushstring(L, configuration::name);
	lua_pushstring(L, configuration::version);
	return 2;
}

int get_executor_name(lua_State* L)
{
	lua_pushstring(L, configuration::name);
	return 1;
}

void other::initialize(lua_State* L)
{
	static const luaL_Reg entries[] = {
		{"getgenv", getgenv},
		{"getreg", getreg},
		{"identifyexecutor", identify_executor},
		{"getexecutorname", get_executor_name},
		{nullptr, nullptr}
	};

	lua_pushvalue(L, LUA_GLOBALSINDEX);
	luaL_register(L, nullptr, entries);

	lua_pop(L, 1);

	script_scheduler->add_to_queue(R"(
		local run_service = game:GetService("RunService")

		getgenv().getfpscap = function()
			return workspace:GetRealPhysicsFPS()
		end

		local current_framerate, _task = nil, nil
		getgenv().setfpscap = function(cap)
			if _task then
				task.cancel(_task)
				_task = nil
			end

			if cap and cap > 0 and cap < 10000 then
				current_framerate = cap
				local interval = 1 / cap

				_task = task.spawn(function()
					while true do
						local start = os.clock()
						run_service.Heartbeat:Wait()
						while os.clock() - start < interval do end
					end
				end)
			else 
				current_framerate = nil
			end
		end

		getgenv().lz4compress = function(data)
			local out, i, data_len = {}, 1, #data
			while i <= data_len do
				local best_len, best_dist = 0, 0

				for dist = 1, math.min(i - 1, 65535) do
					local match_start, len = i - dist, 0
					while i + len <= data_len and data:sub(match_start + len, match_start + len) == data:sub(i + len, i + len) do
						len += 1
						if len == 65535 then break end
					end

					if len > best_len then best_len, best_dist = len, dist end
				end

				if best_len >= 4 then
					table.insert(out, string.char(0) .. string.pack(">I2I2", best_dist - 1, best_len - 4))
					i += best_len
				else
					local lit_start = i

					while i <= data_len and (i - lit_start < 15 or i == data_len) do i += 1 end
					table.insert(out, string.char(i - lit_start) .. data:sub(lit_start, i - 1))
				end
			end
			return table.concat(out)
		end

		getgenv().lz4decompress = function(data, size)
			local out, i, dataLen = {}, 1, #data
			while i <= dataLen and #table.concat(out) < size do
				local token = data:byte(i)
				i = i + 1
				if token == 0 then
					local dist, len = string.unpack(">I2I2", data:sub(i, i + 3))

					i = i + 4
					dist = dist + 1
					len = len + 4

					local start = #table.concat(out) - dist + 1
					local match = table.concat(out):sub(start, start + len - 1)
					while #match < len do
						match = match .. match
					end
					table.insert(out, match:sub(1, len))
				else
					table.insert(out, data:sub(i, i + token - 1))
					i = i + token
				end
			end
			return table.concat(out):sub(1, size)
		end
	)");
}