

# Raspbery Pi with VFD 16LF01UA3 display.
This is project to run VFD 16 segment display with raspberry Pi (any model) in bitbanning mode
Application to drive VFD is written in C++, also added script that uses `fortune` linux tool to source text. 
All is delivered as deamon (systemD), and start yo play right after linux start.

![Raspbery Pi with VFD display on plexy](/../master/rpi_vfd.gif)

## Circuit:

```
connection VFD display to Raspbery pi GPIO
 rpi                          VFD_16LF01UA3
 ---|                       |------
    |             +5V(1,2)--|
    |-- VFD_RST -- RST(10)--|
    |-- VFD_CLK -- SCLK(8)--|
    |-- VFD_DTA -- DATA(9)--|
    |           GND(11,12)--|
 ---|                       |-------
```

## Compilation & instalation :
```
make && sudo make install
```

## Dependencies: 
  
pigpio library : https://github.com/joan2937/pigpio

## Aplication
Aplication vfd is tool to drive vfd display from shell comand line
Avaiable options : 

```
Program to manage text string to VFD display VFD_16LF01UA3 connected to gpio pins
./vfd [-icm], [-s <txt>], [-r <txt>], [-w <txt>], [-b <0-31>], [-d <txt>] 
Options:
 -i        :Initialize VFD display( reset)
 -c        :Clear display
 -m        :enable swirl effect on displayed text
 -b        :set brightness in range 0 to 31 
 -s <txt>  :display static text max 16 chars 
 -r <txt>  :rotate given text in CCW driection. Unlimited length. 
 -w <txt>  :rotate given text in CW driection. Unlimited length. 
 -d <Num>  :set delay between rotating characters in us. Default is 100000us
```
Aplication have to be run with root privilages

## OS:
In this project I used Manjaro ARM linux, minimal image, run on Raspberry Pi 3 B+
https://manjaro.org/download/

## Service
project offers shell based script as demo `vfd_display.sh` also run as systemD service depend from multiuser target
Please see details in `vfd.unit`.
Compilation target `make install` install this service and enable it. 

## TODO:
- [ ] :pushpin:fix CW rtotation when there is "." or "," in text
- [x] optimize rotation code (partially done)
- [ ] maybe use gpio file access, instead of pigpio lib 
