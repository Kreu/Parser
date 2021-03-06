#pragma once
#include "stdafx.h"
#include "File.h"
#include "Header.h"
#include "Feature.h"

class Parser {
	
protected:
	File file_;
	std::vector<std::shared_ptr<Feature>> features_;
	std::shared_ptr<Header> header_;
public:
	using LinkerMap = std::map<std::string, std::vector<std::string>>;
	using FeatureMap = std::vector<std::shared_ptr<Feature>>;

	explicit Parser(const std::string& filename);
	virtual ~Parser() {};
	Parser& operator= (const Parser& rhs) = delete;

	virtual std::shared_ptr<Header>& GetHeader() = 0;
	virtual FeatureMap& GetAllFeatures() = 0;
	virtual FeatureMap GetFeatureByType(const std::string& feature_type) = 0;
	virtual LinkerMap FindInterdomainLinkers (FeatureMap& domains, FeatureMap& modules, size_t add_to_begin = 0, size_t add_to_end = 0) = 0;

};
