#pragma once
#include "../library.hpp"

class metatable : public library_t
{
public:
	void initialize(lua_State* L) override;
};