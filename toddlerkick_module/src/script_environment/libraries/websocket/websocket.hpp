#pragma once
#include "../library.hpp"

class websocket : public library_t
{
public:
	void initialize(lua_State* L) override;
};