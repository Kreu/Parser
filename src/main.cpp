// NRPS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "ParserTests.h"
//#include "GenBankParserTests.h"

#include "GenBankParser.h"
#include "Header.h"
#include "Feature.h"
#include "GenBankFeature.h"

//Include for testing purposes
#include "ParserTests.h"
#include "GenBankParserTests.h"

int main()
{
	auto t1 = std::chrono::high_resolution_clock::now();

	//Run tests
	const bool TESTS_ENABLED = 0;
	if (TESTS_ENABLED) {
		ParserTests::TestAll();
		GenBankParserTests::TestAll();
	}

	std::experimental::filesystem::path path = "C:\\Users\\Valdeko\\Desktop\\BGC_downloader\\New_Sequences";
	//std::experimental::filesystem::path path = "C:\\Users\\Valdeko\\source\\repos\\NRPS\\Sequences";
	//std::experimental::filesystem::path path = "C:\\Users\\Valdeko\\source\\repos\\NRPS\\Tests";

	/*Parser::LinkerMap all_linkers;*/
	int counter{ 1 };
	for (auto& file : std::experimental::filesystem::directory_iterator(path)) {
		try {
			Parser& parser = GenBankParser(file.path().string());
			std::vector<std::shared_ptr<Feature>> modules, domains;

			std::vector<std::shared_ptr<Feature>> CDS_features = parser.GetFeatureByType("CDS");
			//Find all 'modules' which are CDS features that have been marked biosynthetic
			for (const auto& CDS_feat : CDS_features) {
				if (CDS_feat->Find("sec_met", "Kind: biosynthetic")) {
					modules.push_back(std::move(CDS_feat));
				}
			}
			//Find all 'domains' which are aSDomain features.
			std::vector<std::shared_ptr<Feature>> aSDomains = parser.GetFeatureByType("aSDomain");
			for (const auto& as_domains : aSDomains) {
				domains.push_back(std::move(as_domains));
			}
			Parser::LinkerMap linkers = parser.FindInterdomainLinkers(domains, modules);

			//Will create duplicates because same linker can be in different clusters.
			for (const auto& l : linkers) {
				std::string file_name = ".\\Linkers\\" + l.first + ".txt";
				std::ofstream output_file(file_name, std::ios::out | std::ios::app);
				std::set<std::string> unique_linkers{ l.second.begin(), l.second.end() };
				for (const auto& x : unique_linkers) {
					output_file << ">" << counter << "\n";
					output_file << x << "\n";
					++counter;
				}
			}
		}
		catch (std::runtime_error& e) {
			std::cout << e.what() << "\n";
		}
	}

	counter = 1;
	//Fix this..This is idiotic but will do for now. 
	std::experimental::filesystem::path linker_files = "C:\\Users\\Valdeko\\source\\repos\\NRPS\\NRPS\\Linkers";
	for (auto& file : std::experimental::filesystem::directory_iterator(linker_files)) {
		std::ifstream input_file(file, std::ios::in);
		std::string current_line;
		std::set<std::string> unique_linkers;

		std::string filename = file.path().string();
		filename = filename.substr(0, filename.length() - 4);
		std::cout << filename << "\n";

		while (getline(input_file, current_line)) {
			if (current_line.find(">")) {
				std::cout << "Appending " << current_line << "\n";
				unique_linkers.insert(current_line);
			}
		}

		std::ofstream output_file(file);
		for (const auto& x : unique_linkers) {
			output_file << ">" << counter << "\n";
			output_file << x << "\n";
			++counter;
		}
	}


	//std::map<int, int> a = { {0, 4}, {1, 6}, {3, 10} };
	//std::map<int, int> b = { { 2, 8 },{ 1, 11 },{ 3, 10 } };
	//a.insert(b.begin(), b.end());
	//for (const auto& x : a) {
	//	std::cout << x.first << " : " << x.second << "\n";
	//}


	////for (const auto& l : all_linkers) {
	////	std::cout << "Linker type: " << l.first << "\n";
	////	for (const auto& x : l.second) {
	////		std::cout << x << "\n";
	////	}
	////}

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << "Execution took " << duration << " milliseconds\n";
    return 0;
}

