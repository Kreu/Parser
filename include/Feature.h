#pragma once
#include "stdafx.h"

class Feature {
//For testing purposes
friend class GenBankParserTests;

protected:
	std::string type_;
	std::map<std::string, std::string> content_;
	int start_location;;
	int stop_location;
public:
	Feature() = default;
	virtual ~Feature() = 0;
	virtual void PrintFeature() = 0;

	const std::string& GetType();
	const std::map<std::string, std::string>&  GetContent();
	bool Find(const std::string& qualifier) const;
	bool Find(const std::string& qualifier, const std::string& search_term) const;
	const std::string& Feature::GetQualifierContent(const std::string& qualifier);
};