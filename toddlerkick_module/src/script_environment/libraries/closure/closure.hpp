#pragma once
#include "../library.hpp"
#include <script_scheduler/script_scheduler.hpp>

class closure : public library_t
{
public:
	void initialize(lua_State* L) override;
};