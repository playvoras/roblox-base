#pragma once
#include <Windows.h>

#include <lualib.h>
#include <lobject.h>
#include <lapi.h>
#include <lgc.h>

#include <configuration.hpp>

class library_t
{
public:
	virtual void initialize(lua_State* L) {};
};