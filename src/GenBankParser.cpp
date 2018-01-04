#include "stdafx.h"
#include "GenBankParser.h"
#include "Header.h"
#include "Feature.h"
#include "GenBankFeature.h"

using KeywordSpacer = int;

GenBankParser::GenBankParser(const std::string& filename) : Parser(filename) {
	ParseHeader();
	ParseFeatures();
}

void GenBankParser::ParseHeader() {
	//Extracts the header from a GenBank file.

	//Here is an example of a GenBank file header:

	//LOCUS       SCU49845     5028 bp    DNA             PLN       21-JUN-1999
	//DEFINITION  Saccharomyces cerevisiae TCP1-beta gene, partial cds, and Axl2p
	//(AXL2) and Rev7p (REV7) genes, complete cds.
	//ACCESSION   U49845
	//VERSION     U49845.1  GI:1293613
	//KEYWORDS    .
	//SOURCE      Saccharomyces cerevisiae (baker's yeast)
	//  ORGANISM  Saccharomyces cerevisiae
	//			Eukaryota; Fungi; Ascomycota; Saccharomycotina; Saccharomycetes;
	//			Saccharomycetales; Saccharomycetaceae; Saccharomyces.
	//REFERENCE   1  (bases 1 to 5028)
	//  AUTHORS   Torpey,L.E., Gibbs,P.E., Nelson,J. and Lawrence,C.W.
	//  TITLE     Cloning and sequence of REV7, a gene whose function is required for
	//			DNA damage-induced mutagenesis in Saccharomyces cerevisiae
	//  JOURNAL   Yeast 10 (11), 1503-1509 (1994)
	//  PUBMED    7871890
	//REFERENCE   2  (bases 1 to 5028)
	//  AUTHORS   Roemer,T., Madden,K., Chang,J. and Snyder,M.
	//  TITLE     Selection of axial growth sites in yeast requires Axl2p, a novel
	//			plasma membrane glycoprotein
	//  JOURNAL   Genes Dev. 10 (7), 777-793 (1996)
	//  PUBMED    8846915
	//REFERENCE   3  (bases 1 to 5028)
	//  AUTHORS   Roemer,T.
	//  TITLE     Direct Submission
	//  JOURNAL   Submitted (22-FEB-1996) Terry Roemer, Biology, Yale University, New
	//			Haven, CT, USA

	//A header ends and the next keyword is:
	//FEATURES             Location/Qualifiers

	//The code loops through the file and finds the keywords. It creates a GenBank 
	//object that is a wrapper for all of the data contained within.

	std::string current_line, header_keyword;
	std::map<std::string, std::vector<std::string>> header_content;
	std::fstream& file = file_.GetStream();

	while (getline(file, current_line)) {

		bool found_header_keyword = false;
	
		//If we hit "FEATURE", this means we are out of the header and need to stop.
		if (current_line.find("FEATURES") != std::string::npos) {
			header_ = std::make_unique<Header>(Header(header_content));

			//Set the file stream back to beginning so that it is ready to be used
			//by other funtions.
			file.clear();
			file.seekg(0, std::ios_base::beg);

			return;
		}

		//Detect whether the line contains a keyword. The keywords are guaranteed 
		//to be 12 characters wide and always at the beginning of a line. Easiest
		//way is to grab the first 12 characters and see if it's all spaces or not.
		constexpr int HEADER_KEYWORD_WIDTH = 12;
		std::string search_for_keyword = current_line.substr(0, HEADER_KEYWORD_WIDTH);
		if (search_for_keyword.find_first_not_of(' ') != std::string::npos) {
			//Set the flag that we found the keyword
			found_header_keyword = true;
			header_keyword = current_line.substr(0, HEADER_KEYWORD_WIDTH);
			header_keyword.erase(std::remove_if(header_keyword.begin(), header_keyword.end(), isspace), header_keyword.end());

			//Start grabbing the line content past the header keyword width
			std::string line_content{ current_line.substr(HEADER_KEYWORD_WIDTH) };
			header_content[header_keyword].push_back(line_content);
		}

		//If we did not find a header keyword, the next line is part of a multiline
		//comment so we need to append that to the currently tracked header keyword.
		if (!found_header_keyword) {
			std::string line_content{ current_line.substr(HEADER_KEYWORD_WIDTH) };
			header_content[header_keyword].push_back(line_content);
		}
	}
};

void GenBankParser::ParseFeatures() {
	//Extracts information about the sequence features from a GenBank file.

	//Sample of features in a file:

	//FEATURES             Location/Qualifiers
	//cluster         1..49384
	//				/clusterblast="2. JKYW01000001_c1	Mycobacterium
	//				tuberculosis MAL020157 adOYz-supercont1.1.C1... (93% of
	//				genes show similarity)"
	//				/clusterblast="3. JKGQ01000001_c1	Mycobacterium
	//				tuberculosis BTB06-001 adOWX-supercont1.1.C1... (93% of
	//				genes show similarity)"
	//				/clusterblast="4. JKFG01000001_c1	Mycobacterium
	//				tuberculosis BTB08-283 adTxx-supercont1.1.C1... (93% of
	//				genes show similarity)"
	//				/clusterblast="5. JJSH01000013_c1	Mycobacterium
	//				tuberculosis NRITLD20 adPad-supercont1.11.C1... (93% of
	//				genes show similarity)"
	//				/clusterblast="6. CP002992_c1	Mycobacterium tuberculosis
	//				CTRI-2, complete genome. (93% of genes show similarity)"
	//				/clusterblast="7. JKFP01000001_c1	Mycobacterium
	//				tuberculosis BTB08-001 adTxi-supercont1.1.C1... (93% of
	//				genes show similarity)"
	//				/clusterblast="8. JKUF01000001_c1	Mycobacterium
	//				tuberculosis TB_RSA87 adOVU-supercont1.1.C1,... (93% of
	//				genes show similarity)"
	//				/clusterblast="9. JKUE01000001_c1	Mycobacterium
	//				tuberculosis TB_RSA88 adOVy-supercont1.1.C1,... (93% of
	//				genes show similarity)"
	//				/clusterblast="10. JKTG01000053_c1	Mycobacterium
	//				tuberculosis TB_RSA113 adOVe-supercont1.7.C... (93% of
	//				genes show similarity)"
	//				/clusterblast="11. JKSG01000017_c1	Mycobacterium
	//				tuberculosis TB_RSA141 adPeA-supercont1.5.C... (93% of
	//				genes show similarity)"
	//				/note="Cluster number: 1"
	//				/note="Detection rule(s) for this cluster type: nrps:
	//				((Condensation & AMP-binding) or (Condensation & A-OX) or
	//				cluster(Condensation,AMP-binding));"
	//				/note="Monomers prediction: (nrp) + (lys-phe)"
	//				/note="Structure image: structures/genecluster1.png"
	//				/product="nrps"
	//gene            49..870
	//				/locus_tag="MT0086"
	//CDS             49..870
	//				/codon_start=1
	//				/db_xref="GI:13879134"
	//				/locus_tag="MT0086"
	//				/note="identified by Glimmer2; putative"
	//				/product="hypothetical protein"
	//				/protein_id="AAK44311.1"
	//				/transl_table=11
	//				/translation="MEPKRSRLVVCAPEPSHAREFPDVAVFSGGRANASQAERLARAVG
	//				RVLADRGVTGGARVRLTMANCADGPTLVQINLQVGDTPLRAQAATAGIDDLRPALIRLD
	//				RQIVRASAQWCPRPWPDRPRRRLTTPAEALVTRRKPVVLRRATPLQAIAAMDAMDYDVH
	//				LFTDAETGEDAVVYRAGPSGLRLARQHHVFPPGWSRCRAPAGPPVPLIVNSRPTPVLTE
	//				AAAVDRAREHGLPFLFFTDQATGRGQLLYSRYDGNLGLITPTGDGVADGLA"

	bool parsing_a_feature = false;
	bool found_feature_keyword = false;
	bool in_features_section = false;

	std::string current_line, feature_keyword, current_feature_content;
	std::map<std::string, std::vector<std::string>> feature_content;
	std::fstream& file = file_.GetStream();

	while (getline(file, current_line)) {

		//Make sure we are in the Features section
		if (current_line.find("FEATURES") != std::string::npos) {
			in_features_section = true;
			continue;
		}

		if (!in_features_section) {
			continue;
		}

		//If we hit "ORIGIN", this means we are out of the feature and need to stop.
		//Build the last Feature object and quit
		if (current_line.find("ORIGIN") != std::string::npos) {
			//feature_content[feature_keyword].push_back(current_feature_content);
			try {
				std::shared_ptr<Feature> p_feature;
				p_feature = std::make_shared<GenBankFeature>(GenBankFeature(feature_content));
				features_.push_back(p_feature);
				feature_content.clear();

				//Set the file stream back to beginning so that it is ready to be used
				//by other funtions.
				file.clear();
				file.seekg(0, std::ios_base::beg);

				return;
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what();
				return;
			}
			return;
		}

		found_feature_keyword = false;

		//Detect whether the line contains a keyword. The keywords are guaranteed 
		//to be 21 characters wide and always at the beginning of a line. Easiest
		//way is to grab the first 21 characters and see if it's all spaces or not.
		constexpr int FEATURE_KEYWORD_WIDTH = 21;
		std::string search_for_keyword = current_line.substr(0, FEATURE_KEYWORD_WIDTH);
		if (search_for_keyword.find_first_not_of(' ') != std::string::npos) {
			
			//Set the flag that we found the keyword
			found_feature_keyword = true;

			//If we find a feature keyword but have already been reading in a
			//feature, we have to create a new Feature object.
			if ((parsing_a_feature == true) && (found_feature_keyword == true)) {
				try {
					std::shared_ptr<Feature> p_feature;
					p_feature = std::make_shared<GenBankFeature>(GenBankFeature(feature_content));
					features_.push_back(p_feature);
					feature_content.clear();
				}
				catch (const std::invalid_argument& e) {
					std::cout << e.what();
				}
			}

			//Set the flag that we are parsing a feature
			parsing_a_feature = true;

			feature_keyword = current_line.substr(0, FEATURE_KEYWORD_WIDTH);
			feature_keyword.erase(std::remove_if(feature_keyword.begin(), feature_keyword.end(), isspace), feature_keyword.end());

			//Start grabbing the line content past the header keyword width
			std::string line_content{ current_line.substr(FEATURE_KEYWORD_WIDTH) };
			feature_content[feature_keyword].push_back(line_content);
		}

		//If we did not find a feature keyword, the next line is part of a multiline
		//comment so we need to append that to the currently tracked header keyword.
		if (!found_feature_keyword) {
			std::string line_content{ current_line.substr(FEATURE_KEYWORD_WIDTH) };
			feature_content[feature_keyword].push_back(line_content);
		}
	}
};

std::vector<std::shared_ptr<Feature>>& GenBankParser::GetAllFeatures() {
	return features_;
}

std::vector<std::shared_ptr<Feature>> GenBankParser::GetFeatureByType(const std::string& feature_type) {
	std::vector<std::shared_ptr<Feature>> matching_features;
	for (const auto& feat : features_) {
		if (feat->GetType() == feature_type) {
			matching_features.push_back(feat);
		}
	}
	return matching_features;
}

std::shared_ptr<Header>& GenBankParser::GetHeader() {
	return header_;
}

Parser::LinkerMap GenBankParser::FindInterdomainLinkers (FeatureMap& domains, FeatureMap& modules, int add_to_begin, int add_to_end) {

	//TO-DO
	//Implement add_to_begin and add_to_end to get longer linkers that "eat"
	//into the domains.

	std::map<std::string, std::vector<std::string>> linkers;

	for (const auto& m : modules) {
		const auto& m_tag = m->GetQualifierContent("locus_tag");
		const auto& m_trans = m->GetQualifierContent("translation");

		//Iterate through the domains and find the starting positions and
		//lenghts of these domains in each module.
		std::vector<std::tuple<size_t, size_t, std::string>> domain_location_length_type;
		std::string domain_type;

		for (const auto& d : domains) {
			domain_type = d->GetQualifierContent("domain");
			const auto& d_tag = d->GetQualifierContent("locus_tag");
			if (m_tag == d_tag) {
				const auto&  d_trans = d->GetQualifierContent("translation");
				std::size_t domain_start_pos = m_trans.find(d_trans);
				if (domain_start_pos != std::string::npos) {
					auto domain_pos_and_len = std::make_tuple(domain_start_pos, d_trans.length(), domain_type);
					domain_location_length_type.push_back(std::move(domain_pos_and_len));
				}
			}
		}
		//Sort the vector based on the first element (location)
		//They should already be sorted but just in case they are not...
		std::sort(domain_location_length_type.begin(), domain_location_length_type.end());

		//Have all the domain locations, now find the linkers
		for (auto it = domain_location_length_type.begin(); it != domain_location_length_type.end(); ++it) {
			auto next_element = it + 1;
			std::size_t current_domain_end = std::get<0>(*it) + std::get<1>(*it);
			if (next_element != domain_location_length_type.end()) {
				std::size_t next_domain_start = std::get<0>(*next_element);
				std::size_t linker_distance = next_domain_start - current_domain_end;

				auto current_domain_type = std::get<2>(*it);
				auto next_domain_type = std::get<2>(*next_element);
				std::string linker_type = current_domain_type + "-" + next_domain_type;
				//std::cout << "current domain_type: " << std::get<2>(*it) << "\n";
				//std::cout << "next domain_type: " << std::get<2>(*next_element) << "\n";

				linkers[linker_type].push_back(m_trans.substr(current_domain_end, linker_distance));
				//std::cout << "Current end: " << current_domain_end << ". Next start: " << next_domain_start << ". Linker length: " << linker_distance << "\n";
				//std::cout << "Tag: " << m_tag << ". Interdomain linker is " << m_trans.substr(current_domain_end, linker_distance) << "\n";
				continue;
			}
		}
	}
	return linkers;
}