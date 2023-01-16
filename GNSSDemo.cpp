/*  USC RPL HAMSTER v2.3 U-Blox GPS Tests
    Author: Vivek Ramachandran <vsramach@usc.edu>
    Jamie Smith
    Adhyyan Sekhsaria
 */

#include "BarGraph.h"

#include "GNSSDemo.h"
#include "GNSSConstructor.h"

#include <array>
#include <cinttypes>
#include <cmath>


using namespace UBlox;

/**
 * @brief      Constructs the object.
 */
GNSSDemo::GNSSDemo():
gnss(constructGNSS())
{
    bool sensorInitialized = gnss.begin(false);
    if(!sensorInitialized)
    {
        printf("begin():          FAIL!\r\n");
        printf(" -> Failed to contact GPS.\r\n");
    }
    else
    {
        printf("begin():          PASS\r\n");
        gnss.checkVersion(true, false);
    }
}

void GNSSDemo::test_readGNSSConfig() {
    gnss.printGNSSConfig();
}

void GNSSDemo::test_configure()
{
    Timer configTimer;
    configTimer.start();

    if(gnss.configure())
    {
        printf("Successfully configured in %" PRIi64 " ms\r\n", configTimer.elapsed_time().count());
    }
    else
    {
        printf("Configuration failed.\r\n");
    }
}

void GNSSDemo::test_antennaPower() {

	Timer measureTimer;
	measureTimer.start();
    uint8_t powerStat = static_cast<uint8_t>(gnss.getAntennaPowerStatus());
    measureTimer.stop();
    const char* status[] = {"OFF", "ON", "DK", "FAIL TO COMMUNICATE"};
    printf("Read antenna current power status as %s in %" PRIi64 " ms\r\n",
        status[powerStat],
        measureTimer.elapsed_time().count());
}

#define UPDATE_RATE 1s // seconds
#define TERMINAL_WIDTH 80 // chars
#define MAX_SATELLITES 32
#define GRAPH_HEIGHT 20 // lines

#define VT100_CLEAR_SCREEN "\x1b[2J"
#define VT100_CURSOR_HOME "\x1b[H"


bool shouldSkipSatellite(SatelliteInfo & satellite)
{
	return !satellite.svUsed;
}

SatelliteInfo satelliteInfo[MAX_SATELLITES];
uint32_t heights[40];
std::string labels[40][2];

void GNSSDemo::test_HUD()
{

	Timer updateTimer;
	updateTimer.start();

	//initialise bar graph
	BarGraph bargraph;
	bargraph.setAxis("Carrier to Noise Ratio", "db", "SAT ID");
	bargraph.setGraphHeight(40);
	bargraph.setLabelRows(2);
    bargraph.setScalingValue(0);

	bargraph.setHeights(heights);
	bargraph.setLabels(&labels[0][0]);

    FileHandle *stdin_fh = mbed_file_handle(STDIN_FILENO);
    stdin_fh->set_blocking(false);

	while(!stdin_fh->readable()) // exit when the user types a char
	{
		while(updateTimer.elapsed_time() < UPDATE_RATE)
		{
            ThisThread::sleep_for(1ms);
		}
		updateTimer.reset();

		// get the data we need
		gnss.update(200ms);
		ssize_t numSatellites = gnss.getSatelliteInfo(satelliteInfo, MAX_SATELLITES);

		if(numSatellites < 0)
		{
			// error reading satellites, assume 0 satellites
			numSatellites = 0;
		}

		// paint the screen
		printf(VT100_CLEAR_SCREEN VT100_CURSOR_HOME);

		// Title
		printf("---------- U-Blox GPS HUD ---------- (press any key to exit)\r\n");

		// Position
		printf(">Position: %.06f %c, %.06f %c, Height %.02f m\r\n",
				std::abs(gnss.position.latitude), gnss.position.latitude > 0 ? 'N' : 'S',
				std::abs(gnss.position.longitude), gnss.position.longitude > 0 ? 'E' : 'W',
				gnss.position.height / 1000.0f);

		// velocity
		printf(">Velocity: %.02f m/s N, %.02f m/s E, %.02f m/s Down\r\n",
				gnss.velocity.northVel * .02f,
				gnss.velocity.eastVel * .02f,
				gnss.velocity.downVel * .02f);

		// accuracy
		printf(">Fix: Quality: %" PRIu8 ", Num Satellites: %" PRIu8 ", Position Accuracy: %" PRIu32 " cm\r\n",
				static_cast<uint8_t>(gnss.fixQuality.fixQuality),
                gnss.fixQuality.numSatellites,
                gnss.fixQuality.posAccuracy);

		// time
		printf(">Time: %" PRIu8 "/%" PRIu8"/%" PRIu16" %" PRIu8":%" PRIu8 ":%" PRIu8 "\r\n",
				gnss.time.month,
				gnss.time.day,
				gnss.time.year,
				gnss.time.hour,
				gnss.time.minute,
				gnss.time.second);

		// graph
		int currentIndex = 0;
		for(ssize_t index = 0; index < numSatellites; index++){
		    if(shouldSkipSatellite(satelliteInfo[index])){
		        continue;
		    }

		    heights[currentIndex] = satelliteInfo[index].signalStrength;
		    labels[currentIndex][0] = std::to_string(satelliteInfo[index].satelliteID);
		    labels[currentIndex][1] = satelliteInfo[index].getGNSSName();
		    currentIndex++;
		}

		bargraph.setNumCols(currentIndex);
		bargraph.print();
	}

    stdin_fh->set_blocking(true);
}

void GNSSDemo::hotStartTest()
{
	gnss.softwareReset(UBloxGPS::SWResetType::HOT_START);
	gnss.begin(true);
}

#if HAMSTER_SIMULATOR != 1
int main()
#else
int gps_test_main()
#endif
{

    //declare the test harness
    GNSSDemo harness;
    while(1){
        int test=-1;
        printf("\r\n\nGNSS Demo Test Suite:\r\n");

        //MENU. ADD AN OPTION FOR EACH TEST.
        printf("Select a test: \n\r");
        printf("0.  Exit test suite\r\n");
        printf("1.  Reconfigure GNSS settings (must be done first to get data from GNSSs that have not been configured or those without flash memory)\r\n");
        printf("2.  Read antenna power stats\r\n");
        printf("3.  View data in GNSS HUD\r\n");
        printf("4.  Read satellite system status\r\n");
        printf("5.  Software reset GNSS (hot start)\r\n");

        scanf("%d", &test);
        getc(stdin);
        printf("Running test %d:\r\n\n", test);

        //SWITCH. ADD A CASE FOR EACH TEST.
        switch(test) {
            case 0:         printf("Exiting test suite.\r\n");      return 0;
            case 1:         harness.test_configure();               break;
            case 2:         harness.test_antennaPower();            break;
            case 3:         harness.test_HUD();                     break;
            case 4:         harness.test_readGNSSConfig();          break;
            case 5:         harness.hotStartTest();                 break;
            default:        printf("Invalid test number. Please run again.\r\n"); break;
        }
        printf("done.\r\n");
    }
    return 0;
}
