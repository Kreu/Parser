#pragma once
#include "stdafx.h"
#include "Feature.h"

class GenBankFeature : public Feature {
//For testing purposes
friend class GenBankParserTests;

private:
	void UnpackFeatureContent(const std::map<std::string, std::vector<std::string>>& content);
	void FixTranslationField();
	void GenBankFeature::ReverseSpaceTrim(std::string& s);
	void GenBankFeature::RemoveSpacesFromEnd();
	std::string RemoveQuotationMarks(const std::string& line);
	std::pair<std::string, std::string> GenBankFeature::GetTypeAndContent(const std::string& line);
public:
	GenBankFeature(const std::map<std::string, std::vector<std::string>>& feature_content);
	~GenBankFeature() = default;
	void PrintFeature() override;
};