#pragma once
#include "stdafx.h"

class GenBankParserTests {
public:
	static void TestParseHeader();
	static void TestParseFeatures();
	static void TestRemoveQuotationMarks();
	static void TestGetTypeAndContent();
	static void TestAll();
};