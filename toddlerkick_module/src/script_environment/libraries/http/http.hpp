#pragma once
#include "../library.hpp"

class http : public library_t
{
public:
	void initialize(lua_State* L) override;
	auto http_get( lua_State* l ) -> int;
	auto download_string( std::string url ) -> std::string;
};