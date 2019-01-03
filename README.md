# Ozcy, the OP-Z Companion

This repo contains development and source files for Ozcy, an unofficial OP-Z extension module.

Ozcy's goal is to 

* Provide graphical feedback on OP-Z data, such as parameter and step details
* Provide macro features like mute group changes through additional input methos, like buttons or foot controllers
* Enable easy patch management without the need for the App
* Provide clock signals for Pocket Operatos
* Be a fun little companion to take along on your OP-Z journeys

## Hardware

Ozcy connects to the OP-Z either via Bluetooth or USB. The latter is preferred when generating PO clocks as it results in lower latency.

## Software



## Development

Since the OP-Z SysEx interface is not officially documented I need to analyze it to figure out the proprietary protocol messages. Some of the analysis information is available in the [midi development section](dev/midi).

