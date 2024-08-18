#pragma once
#include <Windows.h>
#include <filesystem>
#include <fstream>

#include "../library.hpp"
#include <script_scheduler/script_scheduler.hpp>

class file_system : public library_t
{
public:
	void initialize(lua_State* L) override;
};