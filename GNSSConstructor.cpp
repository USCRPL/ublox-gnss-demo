
// Pin definitions for the U-Blox demo project.
// change these to match your board setup

#include <ZEDF9P.h>
#include <MAX8.h>

#include "GNSSConstructor.h"

#warning Construction file not set up yet!  Modify this file to add your constructor, then comment out this warning.

UBlox::UBloxGPS & constructGNSS()
{
    // Implementation for MAX8 over I2C
//    static I2C i2c(PC_1, PC_0);
//    i2c.frequency(400000);
//
//    static UBlox::MAX8I2C gnss(i2c, NC, UBloxGPS_I2C_DEF_ADDRESS);

    // Implementation for ZED-F9P over SPI
    static UBlox::ZEDF9PSPI gnss(PE_14, PE_13, PD_14, PE_12, PD_15, 2500000);

    return gnss;
};