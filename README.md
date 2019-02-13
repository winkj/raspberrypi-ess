# raspberrypi-ess

## Foreword

While this tutorial is specific to a specific platform/sensor combination (Raspberry Pi, SGP30/SHTC1), the generic driver Sensirion offers support a number of hardware platforms (see [here](https://github.com/Sensirion/embedded-common/tree/master/hw_i2c/sample-implementations) and [here](https://github.com/Sensirion/embedded-common/tree/master/sw_i2c/sample-implementations)), so the instructions in here can be adapted to any other combination of supported platform and sensor. More sensor libraries can be found at:

| Sensor | URL |
| ------ | --- |
| SPS30 | https://github.com/Sensirion/embedded-sps |
| SHT3x/SHTW2/SHTC1/SHTC3 | https://github.com/Sensirion/embedded-sht |
| SGP30/SGPC3 | https://github.com/Sensirion/embedded-sgp |
| SCD30 | https://github.com/Sensirion/embedded-scd |

## Introduction

This tutorial describes how to connect the [Sensirion Environmental Shield (ESS)](https://http://developer.sensirion.com/ess) to a [Raspberry Pi](http://raspberrypi.org). The majority of this tutorial can be applied to any setup based on Linux, with only minor differences.

## Hardware needed

The main components are of course the Raspberry Pi itself, and the Sensirion ESS:
1. [Raspberry Pi](http://raspberrypi.org)
1. [Sensirion Environmental Shield (ESS)](https://http://developer.sensirion.com/ess)

In addition, you will need a microSD card setup for Raspberry Pi. We recommend to use Raspbian Stretch Lite, available [here](https://www.raspberrypi.org/downloads/raspbian/).

Finally, we recommend to use a Raspberry Pi to Arduino Adaptor Shield, for example The [ARPI600](https://www.waveshare.com/arpi600.htm), because this is the easiest and safest way to connect the board If you'd rather use jumper wires see [Appendix A](#appendix-a-manually-connecting-the-ess-to-a-raspberry-pi)

## Hardware setup

## System setup

## Drive 


## Appendix A: Manually connecting the ESS to a Raspberry Pi
