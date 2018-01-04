#include "stdafx.h"
#include <cassert>
#include "GenBankParser.h"
#include "ParserTests.h"

void ParserTests::TestFileNotFound() {

	//Open a file that exists.
	std::string filename = "C:\\Users\\Valdeko\\source\\repos\\NRPS\\Debug\\TestGenBankFile.gbk";
	try {
		Parser& parser = GenBankParser(filename);
	}
	catch (std::runtime_error& e) {
		throw e;
	}

	//Try opening a file that is not found
	filename = "C:\\Users\\Valdeko\\source\\repos\\NRPS\\Debug\\FileNotFound.gbk";
	try {
		Parser& parser = GenBankParser(filename);
	}
	catch (std::runtime_error& e) {
		std::string error = e.what();
		assert(error == (filename + " cannot be found."));
	}
}

void ParserTests::TestAll() {
	std::cout << "Testing ParserTests...\n";
	TestFileNotFound();
	std::cout << "All ParserTests passed!\n";
}