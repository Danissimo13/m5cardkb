# Linux M5Stack CardKB driver
A script for using the M5Stack CardKB on Linux

# Setup Steps

## 1. Setting your keyboard to us layout

In order for buttons to return the correct symbols, the keyboard layout will need to be set to us: `sudo nano /etc/default/keyboard` and changing there `XKBLAYOUT` to `us`:

## 2. Connect CardKB to SBC
Connect the appropriate pins according to your device's GPIO documentation.
CardKB pins: [here](https://docs.m5stack.com/en/unit/cardkb)

## 3. Enable i2c
The cardKB communicates over i2c, make sure this is enabled on your SBC. You can find a tutorial on how to do so:

RaspberryPI: [here](https://www.raspberrypi-spy.co.uk/2014/11/enabling-the-i2c-interface-on-the-raspberry-pi/)

Khadas: [here](https://docs.khadas.com/products/sbc/common/applications/gpio/i2c)

## 4. Install Dependencies
- libi2c-dev
- i2c-tools
For example: `sudo apt install libi2c-dev i2c-tools`

## 5. Clone repository, Run and Test
```
git clone https://github.com/Danissimo13/m5cardkb
cd m5cardkb
```
By default program use i2c-5, but if you connect to another, just append device number to executable command
`sudo ./m5cardkb` or `sudo ./m5cardkb {i2c-device-number}`

## 6. Setup service auto-start
In repository folder: `sudo nano m5_cardkb.service`
Change `ExecStart` with full path to `m5_cardkb` file in cloned repository folder.
For example: `/home/khadas/m5cardkb/m5_cardkb`
After that copy `.service` file to `/lib/systemd/system/` by running: `sudo cp m5_cardkb.service /lib/systemd/system/`

Enable and start service:
```
sudo systemctl daemon-reload
sudo systemctl enable m5_cardkb.service
sudo systemctl start m5_cardkb.service
```
It will start auto with system and try connect to CardKB every 3 seconds;

## 7. Customize
You can customize keys by edit source code, build it with
```cpp
gcc m5_cardkb.cpp -li2c
```

Inspired by python version for RaspberryPi: https://github.com/ian-antking/cardkb
