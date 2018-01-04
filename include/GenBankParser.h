#pragma once

#include "stdafx.h"
#include "Parser.h"
#include "Header.h"
#include "Feature.h"

class GenBankParser : public Parser {
private:
	void ParseFeatures();
	void ParseHeader();
	
public:
	explicit GenBankParser(const std::string& filename);
	virtual ~GenBankParser() override = default;

	std::shared_ptr<Header>& GetHeader();
	virtual std::vector<std::shared_ptr<Feature>>& GetAllFeatures() override;
	virtual std::vector<std::shared_ptr<Feature>> GetFeatureByType(const std::string& feature_type) override;
	LinkerMap FindInterdomainLinkers (FeatureMap& domains, FeatureMap& modules, int add_to_begin = 0, int add_to_end = 0) override;
};

