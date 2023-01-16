#pragma once

#include "UBloxGPS.h"

class GNSSDemo
{
    UBlox::UBloxGPS & gnss;

public:

	GNSSDemo();

	void test_configure();

	void test_antennaPower();

	void test_HUD();

	void test_readGNSSConfig();

    void hotStartTest();
};
