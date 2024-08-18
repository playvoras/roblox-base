#include "cache.hpp"

void cache::initialize(lua_State* L)
{
	script_scheduler->add_to_queue(R"(
		local invalidated = {}
		getgenv().cache = {
			invalidate = function(object)
				local old_archivable = object.Archivable
				local clone

				object.Archivable = true
				clone = object:Clone()
				object.Archivable = old_archivable

				local old_parent = object.Parent
				table.insert(invalidated, object)

				object:Destroy()
				clone.Parent = old_parent
			end,
			iscached = function(object)
				return table.find(invalidated, object) == nil
			end,
			replace = function(object, new_object)
				if object:IsA("BasePart") and new_object:IsA("BasePart") then
					invalidate(object)
					table.insert(invalidated, new_object)
				end
			end
		}

		local clones = {}
		getgenv().cloneref = function(object)
			if not clones[object] then clones[object] = {} end
			local clone = {}

			local mt = {
				__type = "Instance",
				__tostring = function()
					return object.Name
				end,
				__index = function(_, key)
					local value = object[key]
					if type(value) == "function" then
						return function(_, ...)
							return value(object, ...)
						end
					else
						return value
					end
				end,
				__newindex = function(_, key, value)
					object[key] = value
				end,
				__metatable = "The metatable is locked",
				__len = function()
					error("attempt to get length of a userdata value")
				end
			}

			setmetatable(clone, mt)
			table.insert(clones[object], clone)

			return clone
		end

		getgenv().compareinstances = function(a, b)
			if clones[a] and table.find(clones[a], b) then
				return true
			elseif clones[b] and table.find(clones[b], a) then
				return true
			else
				return a == b
			end
		end
	)");
}