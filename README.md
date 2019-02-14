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

We will also clone the repository from this tutorial, to get access to a sample application and sample Makefile:

```shell
$ git clone https://github.com/winkj/raspberrypi-ess.git
```

### Step 2: Build driver release

```shell
$ cd embedded-sht
$ make shtc1
$ cd ../embedded-sgp
$ make sgp30
$ cd ..
```

### Step 3: Create your project working directory

```shell
$ mkdir ess-rpi
```

Next, we copy all the necessary files to our project directory
```shell
$ cp embedded-sgp/embedded-common/*.{h,c} ess-rpi
$ cp embedded-sgp/sgp-common/*.{c,h} ess-rpi/
$ cp embedded-sgp/sgp30/*.{c,h} ess-rpi/
$ cp embedded-sht/sht-common/*.{c,h} ess=rpi/
$ cp embedded-sht/shtc1/shtc1.c ess-rpi/
```

Sensirion's sample driver employ a hardware abstraction layer, which enables for simple porting. The code we copied above was completely platform independent, so those steps would be the same for platforms other than Linux/Raspberry Pi. In the next step, we'll bring in the platform specific functions to enable 
```shell
$ cp embedded-sgp/embedded-common/hw_i2c/sample-implementations/linux_user_space/sensirion_hw_i2c_implementation.c ess-rpi/
```

For the next step, we copy over (and rename while we're at it) an example Makefile from this repository
```shell
$ cp raspberrypi-ess/Makefile.example ess-rpi/Makefile
```

As the last step, we need a main function for our application. With the code we copied above, there's currently two examples in our project directory, one from ```embedded-sht``` called ```example_usage.c```, and one from ```embedded-sgp``` called ```sgp30_example_usage.c```. So as a first step, let's rename those so they are out of the way (note that we're removing the ```.c``` suffix; this way, the Makefile will not look at them anymore, which would cause a symbol conflict):
```shell
$ cd ess-rpi
$ mv example_usage.c sht.sample
$ mv sgp30_example_usage.c sgp30.sample
```

To create our final application, we basically combine the two into one. We *recommend* to try this yourself as an exercise. However for convenience or to compare your version against ours, we've also included one in this repository. If you want to use that version, copy it over like this: 
```shell
$ cp ../raspberrypi-ess/
$ cp ../raspberrypi-ess/ess_sample_usage.c .
```

Finally, it's time to compile the sample:
```shell
$ make
# ...
```

If everything worked as expected, you should now have an executable called ```rpi-ess```. Run it to read out the sensors:
```shell
$ ./rpi-ess
# ...
```

## GPIO support

The ESS features three leds (red/orange/green) that can be controlled through the sysfs GPIO interface. When using the ARPI600, the GPIO numbers are as follows:

|GPIO pin|function|
|--------|--------|
| 4|Red LED|
| 8|Orange LED|
|10|Green LED|

### Controlling the LEDs from the shell

To enable GPIO control via the shell, enable GPIO control like this (example using the RED led, pin 4):
```shell
$ echo "4" > /sys/class/gpio/export
$ echo "out" > /sys/class/gpio/gpio4/direction
```

Then, in order to set and clear the GPIO value, simply write ```1``` (= set, turn on the LED) or ```0``` to the corresponding sysfs path:
```shell
$ echo "1" > /sys/class/gpio/gpio4/value
$ echo "0" > /sys/class/gpio/gpio4/value
```

Replace ```4``` and ```gpio4``` with ```8```/```10``` and ```gpio8```/```gpio10``` to control the orange and green LED, respectively.

### Controlling the LEDs from a C application

You can simply reimplement the above shell commands in your C program, or include a GPIO library, for example https://github.com/embeddedarm/gpio-sysfs-demo.

## Questions & Support

If you have any questions on the tutorial, please file an issue here on the github page at https://github.com/winkj/raspberrypi-ess/. If you have a question on Sensirion's product, check out Sensirion's [Support Center](http://sensirion.com/support).

## Appendix A: Manually connecting the ESS to a Raspberry Pi

TBD
