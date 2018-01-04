#include "stdafx.h"
#include "Header.h"

Header::Header(const std::map<std::string, std::vector<std::string>>& content) {
	content_ = content;
}

std::map<std::string, std::vector<std::string>>& Header::GetHeaderContent() {
	return content_;
}

void Header::PrintHeaderContent() {
	for (auto c : content_) {
		std::cout << c.first << "\n";
		for (auto cx : c.second) {
			std::cout << "\t" << cx << "\n";
		}
	}
}
void Header::Clear() {
	content_.clear();
}