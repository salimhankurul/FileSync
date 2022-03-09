#pragma once

#include "Utilities.h"

class Handler
{
public:
	static bool HandlePath(std::filesystem::path FilePath);
	static bool HandleFile(std::filesystem::path FilePath);
	static std::string base_path;
	static std::string username;
};

