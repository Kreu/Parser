#pragma once
#include "stdafx.h"

class File {
private:
	std::fstream file_;
public:
	File(const std::string& filename);
	File(const char* filename);
	std::fstream& GetStream();
};