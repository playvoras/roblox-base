#pragma once
#include "../library.hpp"

class crypt : public library_t
{
public:
	void initialize(lua_State* L) override;
};