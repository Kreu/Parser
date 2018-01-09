#pragma once
#include "stdafx.h"

class File {
private:
	std::fstream file_;
public:
	File(const std::string& filename);
	File(const char* filename);
	File(const File& rhs) = delete;
	File& operator=(const File& rhs) = delete;
	//File(const File& rhs);
	std::fstream& GetStream();
};
