# U-Blox GNSS Demo Project
This project shows how to use Mbed CE's [U-Blox GNSS driver](https://github.com/mbed-ce/ublox-gnss-driver.git) to connect to, configure, and get data from a U-Blox GNSS.   

## What you need:
- U-Blox GNSS.  Currently the MAX-8 and ZED-F9P are supported, but other Gen8 and Gen9 devices should be able to be added easily (they might work out of the box).
  - For example, [this breakout](https://thepihut.com/products/ublox-max-m8q-breakout-for-active-antennas) should work for the MAX-M8.
- GNSS/GPS antenna.
  - If your antenna has an integrated amplifier, ensure that the amplifier is getting power, or your performance will be garbage.  For Gen9 modules that provide internal antenna power, the driver knows to turn this on automatically (not implemented in Gen8 yet!). 
- Any Mbed board with SPI or I2C support

## How to run this demo:

1. Clone it to your machine.  Don't forget to use `--recursive` to clone the submodules: `git clone --recursive https://github.com/mbed-ce/ublox-gnss-demo.git`
2. Set up the GNU ARM toolchain (and other programs) on your machine using [the toolchain setup guide](https://github.com/mbed-ce/mbed-os/wiki/Toolchain-Setup-Guide).
3. Set up the CMake project for editing.  We have three ways to do this:
    - On the [command line](https://github.com/mbed-ce/mbed-os/wiki/Project-Setup:-Command-Line)
    - Using the [CLion IDE](https://github.com/mbed-ce/mbed-os/wiki/Project-Setup:-CLion)
    - Using the [VS Code IDE](https://github.com/mbed-ce/mbed-os/wiki/Project-Setup:-VS-Code)
4. Connect the GNSS to the Mbed board via I2C or (if supported by the GNSS) SPI.  Also connect the antenna to the GNSS.
5. Update GNSSConstructor.h to fill in the GNSS module and pins you are using in your setup.  Then, comment the warning at the top.
6. Build the `flash-GNSSDemo` targets to upload the code to a connected device.
7. Open a serial monitor and access the console.
8. Use the "Reconfigure GNSS settings" menu option to flash the correct configuration onto the GNSS so that it works with this driver.
9. Use the "View data in GNSS HUD" menu option to open a live view of the GNSS data!
