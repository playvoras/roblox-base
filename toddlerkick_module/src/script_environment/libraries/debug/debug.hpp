#pragma once
#include "../library.hpp"

class debug : public library_t
{
public:
	void initialize(lua_State* L) override;
};