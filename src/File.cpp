#include "stdafx.h"
#include "File.h"

File::File(const std::string& filename) {
	//If a filename cannot be found
	file_.open(filename);
	if (!file_.is_open()) {
		throw std::runtime_error(filename + " cannot be found.");
	}
	//Otherwise everything is good
	else {
		std::cout << "Opened " << filename << ".\n";
	}
}

File::File(const char* filename) {
	//If a filename cannot be found
	file_.open(filename);
	if (!file_.is_open()) {
		throw std::runtime_error(std::string(filename) + " cannot be found.");
	}
	//Otherwise everything is good
	else {
		std::cout << "Opened " << filename << ".\n";
	}
}

std::fstream& File::GetStream() {
	return file_;
}

//File::File(const File& rhs) : file_(rhs.file_) { };
