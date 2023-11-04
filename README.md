

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


## OS:
In this project I used Manjaro ARM linux, minimal image, run on Raspberry Pi 3 B+
https://manjaro.org/download/

## TODO:
- [ ] :pushpin:fix CW rtotation when there is "." or "," in text
- [x] optimize rotation code (partially done)
- [ ] maybe use gpio file access, instead of pigpio lib 
