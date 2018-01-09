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
	using DomainInfo = std::vector<std::tuple<size_t, size_t, std::string>>;
	explicit GenBankParser(const std::string& filename);
	virtual ~GenBankParser() override = default;

	std::shared_ptr<Header>& GetHeader();
	virtual std::vector<std::shared_ptr<Feature>>& GetAllFeatures() override;
	virtual std::vector<std::shared_ptr<Feature>> GetFeatureByType(const std::string& feature_type) override;
	LinkerMap FindInterdomainLinkers (FeatureMap& domains, FeatureMap& modules, size_t add_to_begin = 0, size_t add_to_end = 0) override;
	DomainInfo GetDomainStartLengthAndType(const FeatureMap& domains, const std::string& m_tag, const std::string& m_trans);
};

