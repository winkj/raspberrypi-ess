# raspberrypi-ess

## Foreword

While this tutorial is specific to a specific platform/sensor combination (Raspberry Pi, SGP30/SHTC1), the generic driver Sensirion offers support a number of hardware platforms (see [here](https://github.com/Sensirion/embedded-common/tree/master/hw_i2c/sample-implementations) and [here](https://github.com/Sensirion/embedded-common/tree/master/sw_i2c/sample-implementations)), so the instructions in here can be adapted to any other combination of supported platform and sensor. More sensor libraries can be found at:

| Sensor type | Sensor | URL |
| ----------- | ------ | --- |
| Humidity and Temperature | SHT3x/SHTW2/SHTC1/SHTC3 | https://github.com/Sensirion/embedded-sht |
| TotalVOC and CO2 equivalent | SGP30/SGPC3 | https://github.com/Sensirion/embedded-sgp |
| CO2 | SCD30 | https://github.com/Sensirion/embedded-scd |
| Particulate Matter (Dust) | SPS30 | https://github.com/Sensirion/embedded-sps |

More tutorials for Raspberry Pi can be found on our [developer website](https://developer.sensirion.com/platforms/raspberry-pi/).

## Introduction

This tutorial describes how to connect the [Sensirion Environmental Shield (ESS)](https://http://developer.sensirion.com/ess) to a [Raspberry Pi](http://raspberrypi.org). The majority of this tutorial can be applied to any setup based on Linux, with only minor differences.

This tutorial assumes that the reader has basic knowledge of the Raspberry Pi, and a Raspberry Pi setup with working internet access.

For beginners, we recommend Raspberry Pi's {help guides](https://www.raspberrypi.org/help/).

## Hardware needed

The main components are of course the Raspberry Pi itself, and the Sensirion ESS:
1. [Raspberry Pi](http://raspberrypi.org)
1. [Sensirion Environmental Shield (ESS)](https://http://developer.sensirion.com/ess)

In addition, you will need a microSD card setup for Raspberry Pi. This tutorial assumes a Raspbian setup. At the time of writing, Stretch Lite was the latest version, available [here](https://www.raspberrypi.org/downloads/raspbian/).

Finally, we recommend to use a Raspberry Pi to Arduino Adaptor Shield, for example The [ARPI600](https://www.waveshare.com/arpi600.htm), because this is the easiest and safest way to connect the board If you'd rather use jumper wires see [Appendix A](#appendix-a-manually-connecting-the-ess-to-a-raspberry-pi)

## Hardware setup

To setup the hardware, connect the ESS to the ARPI600, and then connect the ARPI600 to the Raspberry Pi

## System setup

**Note:** This tutorial is using the Linux userspace drivers. If you're interested in using kernel drivers, please contact the Sensirion support team.

### Step 1: Enable I2C device Interface
```shell
$ sudo raspi-config
# select ```5 Interfacing Options```, then ```I2C```, and ```YES```
$ ls -l /sys/bus/i2c/devices
# This should output: i2c-1
```

### Step 2 (Optional): Verify the I2C sensors are detected
```shell
$ sudo apt-get install i2c-tools
$ i2cdetect -y 1
# should output at least
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- 58 -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: 70 -- -- -- -- -- -- --  
```
The ```i2cdetect``` commands shows addresses of devices that are connected to the bus. ```0x58``` is the SGP30, ```0x70``` is the SHTC1. You may also see additional devices on there; in particular 0x51 is the RTC included on the ARPI600.

### Step 3: Install necessary software packages

In order to install the drivers, we will to install the ```git``` software packages"
```shell
$ sudo apt-get install git
```

## Driver installation

### Step 1: Clone driver source code

**Tip:** You can execute the following commands anywhere in your filesystem, but the author recommends to create a dedicated project directory to keeps things neatly organized.

To clone the drivers, we'll follow the instructions from https://github.com/Sensirion/embedded-sht and https://github.com/Sensirion/embedded-sgp

```shell
$ git clone --recursive https://github.com/Sensirion/embedded-sht.git
$ git clone --recursive https://github.com/Sensirion/embedded-sgp.git
```

### Step 2: Build driver release


## Appendix A: Manually connecting the ESS to a Raspberry Pi
