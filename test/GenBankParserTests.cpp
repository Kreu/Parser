#include "stdafx.h"
#include <cassert>
#include "Parser.h"
#include "GenBankParser.h"
#include "GenBankParserTests.h"
#include "GenBankFeature.h"

void GenBankParserTests::TestParseHeader() {

	//Test with real input
	try {
		Parser& parser = GenBankParser("C:\\Users\\Valdeko\\source\\repos\\NRPS\\Debug\\TestGenBankFile.gbk");

		std::shared_ptr<Header>& header = parser.GetHeader();
		std::map<std::string, std::vector<std::string>>& expectedHeaderContent = header->GetHeaderContent();

		std::vector<std::string> locusContent = expectedHeaderContent["LOCUS"];
		assert(locusContent[0] == "AE000516               49384 bp    DNA              UNK 01-JAN-1980");

		std::vector<std::string> defContent = expectedHeaderContent["DEFINITION"];
		assert(defContent[0] == "Mycobacterium tuberculosis CDC1551, complete genome.");

		std::vector<std::string> accContent = expectedHeaderContent["ACCESSION"];
		assert(accContent[0] == "AE000516");

		std::vector<std::string> verContent = expectedHeaderContent["VERSION"];
		assert(verContent[0] == "AE000516.2");

		std::vector<std::string> keyContent = expectedHeaderContent["KEYWORDS"];
		assert(keyContent[0] == ".");

		std::vector<std::string> srcContent = expectedHeaderContent["SOURCE"];
		assert(srcContent[0] == ".");

		std::vector<std::string> orgContent = expectedHeaderContent["ORGANISM"];
		assert(orgContent[0] == ".");
		assert(orgContent[1] == ".");
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}

	//Test a manually edited file with random junk in it
	try {
		Parser& parser = GenBankParser("C:\\Users\\Valdeko\\source\\repos\\NRPS\\Debug\\TestGenBankFile2.gbk");
		std::shared_ptr<Header>& header = parser.GetHeader();
		std::map<std::string, std::vector<std::string>>& expectedHeaderContent = header->GetHeaderContent();

		std::vector<std::string> locusContent = expectedHeaderContent["LOCUS"];
		assert(locusContent[0] == "ABC123456 10540bp LOLDNA    UNK 01-JAN-1980");

		std::vector<std::string> defContent = expectedHeaderContent["DEFINITION"];
		assert(defContent[0] == "Totally a real genome");

		std::vector<std::string> accContent = expectedHeaderContent["ACCESSION"];
		assert(accContent[0] == "ABC123456");

		std::vector<std::string> verContent = expectedHeaderContent["VERSION"];
		assert(verContent[0] == "ABC123456.12345");

		std::vector<std::string> keyContent = expectedHeaderContent["KEYWORDS"];
		assert(keyContent[0] == "HMM");

		std::vector<std::string> srcContent = expectedHeaderContent["SOURCE"];
		assert(srcContent[0] == "E. coli");

		std::vector<std::string> orgContent = expectedHeaderContent["ORGANISM"];
		assert(orgContent[0] == "E.coli");
		assert(orgContent[1] == "and Sf9");
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
}

void GenBankParserTests::TestParseFeatures() {
	//Test files are set up to contain a header, ONE feature, and origin.

	//To-Do
	//Add testing for the feature LOCATION

	//Test for "CDS" Feature
	try {
		Parser& parser = GenBankParser("C:\\Users\\Valdeko\\source\\repos\\NRPS\\Debug\\TestFeatureCDS.gbk");
		const std::vector<std::shared_ptr<Feature>>& features = parser.GetAllFeatures();

		for (auto& c : features) {
			assert(c->type_ == "CDS");

			//This is what the content_ should be
			std::map<std::string, std::string> expected_content {
				{ "codon_start", "1" },
				{ "db_xref", "GI:13879158" },
				{ "locus_tag", "MT0111" },
				{ "note", "identified by similarity to EGAD:32940" },
				{ "product", "conserved hypothetical protein" },
				{ "protein_id", "AAK44333.1" },
				{ "transl_table", "11" },
				{ "translation", "MGTHGATKSATSAVPTPRSNSMAMVRLAIGLLGVCAVVAAFGLVSGARRYAEAGNPYPGAFVSVAEPVGFFAASLAGALCLGALIHVVMTAKPEPDGLIDAAAFRIHLLAERVSGLWLGLAATMVVIQAAHDTGVGPARLLASGALSDSVAASEMARGWIVAAICALVVATALRLYTRWLGHVVLLVPTVLAVVATAVTGNPGQGPDHDYATSAAIVFAVAFATLTGLKIAAALAGTTPSRAVLVTQVTCGALALAYGAMLLYLFIPGWAVDSDFARLGLLAGVILTSVWLFDCWRLLVRPPHAGRRRGGGSGAALAMMAAMASIAAMAVMTAPRFLTHAFTAWDVFLGYELPQPPTIARVLTVWRFDSLIGAAGVVLAIGYAAGFAALRRRGNSWPVGRLIAWLTGCAALVFTSGSGVRAYGSAMFSVHMAEHMTLNMFIPVLLVLGGPVTLALRVLPVTGDGRPPGAREWLTWLLHSRVTTFLSHPITAFVLFVASPYIVYFTPLFDTFVRYHWGHEFMAIHFLVVGYLFYWAIIGIDPGPRRLPYPGRIGLLFAVMPFHAFFGIALMTMSSTVGATFYRSVNLPWLSSIIADQHLGGGIAWSLTELPVIMVIVALVTQWARQDRRVASREDRHADSDYADDELEAYNAMLRELSRMRR"}
			};

			assert(c->content_ == expected_content);
		}
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}

	//Test for "aSDomain" Feature
	try {
		Parser& parser = GenBankParser("C:\\Users\\Valdeko\\source\\repos\\NRPS\\Debug\\TestFeatureAsdomain.gbk");
		const std::vector<std::shared_ptr<Feature>>& features = parser.GetAllFeatures();

		for (auto& c : features) {
			//This is what the content_ should be
			std::map<std::string, std::string> expected_content {
				{ "asDomain_id", "nrpspksdomains_MT0110_Xdom03" },
				{ "database", "nrpspksdomains.hmm" },
				{ "detection", "hmmscan" },
				{ "domain", "TD" },
				{ "evalue", "1.30E-85" },
				{ "locus_tag", "MT0110" },
				{ "score", "278.1" },
				{ "translation", "VLLTGATGFLGRYLVLELLRRLDVDGRLICLVRAESDEDARRRLEKTFDSGDPELLRHFKELAADRLEVVAGDKSEPDLGLDQPMWRRLAETVDLIVDSAAMVNAFPYHELFGPNVAGTAELIRIALTTKLKPFTYVSTADVGAAIEPSAFTEDADIRVISPTRTVDGGWAGGYGTSKWAGEVLLREANDLCALPVAVFRCGMILADTSYAGQLNMSDWVTRMVLSLMATGIAPRSFYEPDSEGNRQRAHFDGLPVTFVAEAIA" }
			};

			assert(c->type_ == "aSDomain");
			assert(c->content_ == expected_content);
		}
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}

	//Test for "gene" Feature
	try {
		Parser& parser = GenBankParser("C:\\Users\\Valdeko\\source\\repos\\NRPS\\Debug\\TestFeatureGene.gbk");
		const std::vector<std::shared_ptr<Feature>>& features = parser.GetAllFeatures();

		for (auto& c : features) {
			//This is what the content_ should be
			std::map<std::string, std::string> expected_content {
				{ "locus_tag", "MT0111" },
			};

			assert(c->type_ == "gene");
			assert(c->content_ == expected_content);
		}
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}

	//Test for "CDS_motif" Feature
	try {
		Parser& parser = GenBankParser("C:\\Users\\Valdeko\\source\\repos\\NRPS\\Debug\\TestFeatureCDSmotif.gbk");
		const std::vector<std::shared_ptr<Feature>>& features = parser.GetAllFeatures();

		for (auto& c : features) {

			//This is what the content_ should be
			std::map<std::string, std::string> expected_content {
				{ "aSTool", "pksnrpsmotif" },
				{ "asDomain_id", "nrpspksmotif_MT0110_0019" },
				{ "database", "abmotifs" },
				{ "detection", "hmmscan" },
				{ "evalue", "6.30E+00" },
				{ "label", "C4_LCL_164-176" },
				{ "locus_tag", "MT0110" },
				{ "motif", "C4_LCL_164-176" },
				{ "note", "NRPS/PKS Motif: C4_LCL_164-176 (e-value: 6.3, bit-score: 0.2)" },
				{ "score", "0.2" },
				{ "translation", "DFAEWLQ" }
			};

			assert(c->type_ == "CDS_motif");
			assert(c->content_ == expected_content);
		}
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}

	//Test for "cluster" Feature
	try {
		Parser& parser = GenBankParser("C:\\Users\\Valdeko\\source\\repos\\NRPS\\Debug\\TestFeatureCluster.gbk");
		const std::vector<std::shared_ptr<Feature>>& features = parser.GetAllFeatures();

		for (auto& c : features) {

			//This is what the content_ should be
			std::map<std::string, std::string> expected_content{
				{ "clusterblast", "2. JKYW01000001_c1	Mycobacterium tuberculosis MAL020157 adOYz-supercont1.1.C1... (93% of genes show similarity) "
														"3. JKGQ01000001_c1	Mycobacterium tuberculosis BTB06-001 adOWX-supercont1.1.C1... (93% of genes show similarity) "
														"4. JKFG01000001_c1	Mycobacterium tuberculosis BTB08-283 adTxx-supercont1.1.C1... (93% of genes show similarity) "
														"5. JJSH01000013_c1	Mycobacterium tuberculosis NRITLD20 adPad-supercont1.11.C1... (93% of genes show similarity) "
														"6. CP002992_c1	Mycobacterium tuberculosis CTRI-2, complete genome. (93% of genes show similarity) "
														"7. JKFP01000001_c1	Mycobacterium tuberculosis BTB08-001 adTxi-supercont1.1.C1... (93% of genes show similarity) "
														"8. JKUF01000001_c1	Mycobacterium tuberculosis TB_RSA87 adOVU-supercont1.1.C1,... (93% of genes show similarity) "
														"9. JKUE01000001_c1	Mycobacterium tuberculosis TB_RSA88 adOVy-supercont1.1.C1,... (93% of genes show similarity) "
														"10. JKTG01000053_c1	Mycobacterium tuberculosis TB_RSA113 adOVe-supercont1.7.C... (93% of genes show similarity) "
														"11. JKSG01000017_c1	Mycobacterium tuberculosis TB_RSA141 adPeA-supercont1.5.C... (93% of genes show similarity)" },
				{ "note", "Cluster number: 1 Detection rule(s) for this cluster type: nrps: ((Condensation & AMP-binding) or (Condensation & A-OX) or cluster(Condensation,AMP-binding)); "
												"Monomers prediction: (nrp) + (lys-phe) Structure image: structures/genecluster1.png" },
				{ "product", "nrps" }
			};

			assert(c->type_ == "cluster");
			assert(c->content_ == expected_content);
		}
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
}
//They actually belong with GenBankFeature
//void GenBankParserTests::TestRemoveQuotationMarks() {
//	std::string double_quotation_marks{ "\"This is a string with double quotation marks!\"" };
//	std::string beginning_quotation_mark{ "\"This is a string with a single quotation mark at the beginning!" };
//	std::string end_quotation_mark{ "This is a string with a single quotation mark at the end!\"" };
//
//	assert(GenBankFeature::RemoveQuotationMarks(double_quotation_marks) == "This is a string with double quotation marks!");
//	assert(GenBankFeature::RemoveQuotationMarks(beginning_quotation_mark) == "This is a string with a single quotation mark at the beginning!");
//	assert(GenBankFeature::RemoveQuotationMarks(end_quotation_mark) == "This is a string with a single quotation mark at the end!");
//}
//
//void GenBankParserTests::TestGetTypeAndContent() {
//
//	auto p = GenBankFeature::GetTypeAndContent("/protein_id=\"BAI29479.1\"");
//	assert(p.first == "protein_id");
//	assert(p.second == "\"BAI29479.1\"");
//
//	p = GenBankFeature::GetTypeAndContent("/type=\"NRPS\"");
//	assert(p.first == "type");
//	assert(p.second == "\"NRPS\"");
//
//}

void GenBankParserTests::TestAll() {
	std::cout << "Testing GenBankParserTests...\n";
	TestParseHeader();
	TestParseFeatures();
	std::cout << "All GenBankParserTests passed!\n";
}