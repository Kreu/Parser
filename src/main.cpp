#include "stdafx.h"
#include "GenBankParser.h"
#include "Header.h"
#include "Feature.h"
#include "GenBankFeature.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>


int main()
{
	//To-DO
	//Get arguments for folder and linker lenghts from the command line
	
	auto t1 = std::chrono::high_resolution_clock::now();
	boost::filesystem::path path = "/media/sf_BGC_downloader/New_Sequences/";
	int counter{ 1 };
	
	for (auto& file : boost::filesystem::directory_iterator(path)) {
		try {
			std::string file_to_open = file.path().string();
			GenBankParser parser(file_to_open);
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
			Parser::LinkerMap linkers = parser.FindInterdomainLinkers(domains, modules, 40, 40);

			//Algorithm creates duplicates because same linker can be in different clusters. So we
			//need to make them unique at least across a module, and later across the file.
			for (const auto& l : linkers) {
				std::string file_name = "/media/sf_BGC_downloader/New_Sequences/Linkers/" + l.first + ".txt";
				boost::filesystem::ofstream output_file(file_name, std::ios::out | std::ios::app);
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
	boost::filesystem::path linker_files = "/media/sf_BGC_downloader/New_Sequences/Linkers/";
	for (auto& file : boost::filesystem::directory_iterator(linker_files)) {
		boost::filesystem::ifstream input_file(file, std::ios::in);
		std::string current_line;
		std::set<std::string> unique_linkers;

		std::string filename = file.path().string();
		filename = filename.substr(0, filename.length() - 4);
		std::cout << filename << "\n";

		while (getline(input_file, current_line)) {
			if (current_line.find(">")) {
				//std::cout << "Appending " << current_line << "\n";
				unique_linkers.insert(current_line);
			}
		}

		boost::filesystem::ofstream output_file(file);
		for (const auto& x : unique_linkers) {
			output_file << ">" << counter << "\n";
			output_file << x << "\n";
			++counter;
		}
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << "Execution took " << duration << " milliseconds\n";
	return 0;
}

