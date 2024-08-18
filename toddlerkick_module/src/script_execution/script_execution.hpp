#pragma once
#include <iostream>

#include <Luau/BytecodeBuilder.h>
#include <Luau/BytecodeUtils.h>
#include <Luau/Compiler.h>
#include <zstd/zstd.h>
#include <zstd/xxhash.h>

#include <globals.hpp>

class script_execution_t
{
public:
	void execute_script(const char* script);
};

inline auto script_execution = std::make_unique<script_execution_t>();