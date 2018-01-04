#pragma once
#include "stdafx.h"

class Header {
private:
	std::map<std::string, std::vector<std::string>> content_;
public:
	Header() {};
	explicit Header(const std::map<std::string, std::vector<std::string>>&);

	std::map<std::string, std::vector<std::string>>& GetHeaderContent();
	void PrintHeaderContent();

	void Clear();
};