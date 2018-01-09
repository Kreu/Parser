#include "stdafx.h"
#include "Feature.h"

Feature::~Feature() {};

const std::string& Feature::GetType() {
	return type_;
}

const std::map<std::string, std::string>& Feature::GetContent() {
	return content_;
}

bool Feature::Find(const std::string& qualifier) const {
	if (content_.find(qualifier) != content_.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool Feature::Find(const std::string& qualifier, const std::string& search_term) const {
	if (content_.find(qualifier) != content_.end()) {
		if (content_.at(qualifier).find(search_term) != std::string::npos) {
			return true;
		}
		return false;
	}
	else {
		return false;
	}
	return false;
}

const std::string& Feature::GetQualifierContent(const std::string& qualifier) {
	if (content_.find(qualifier) != content_.end()) {
		return content_.at(qualifier);
	}
	else {
		return qualifier;
	}
}