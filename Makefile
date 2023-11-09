appname      := vfd

PREFIX       := /usr
LIBINST      := $(PREFIX)/lib
BININST      := $(PREFIX)/bin
SYSTEMDINST  := /lib/systemd/system
SYSTEMDENABLE := /etc/systemd/system/multi-user.target.wants

CCX=g++
CFLAGS=-c -Wall -O2
LDLIBS=-lpigpio
CP=/bin/cp
LN=/bin/ln -sf
RM=/bin/rm -f
all: $(appname) 

.PHONY: all prog vfd.o  VFD_16LF01UA3.o clean install uninstall

$(appname): vfd.o  VFD_16LF01UA3.o
	$(CCX) $(LDLIBS) vfd.o  VFD_16LF01UA3.o -o $(appname)

clean:
	$(RM) *.o $(appname)

vfd.o:
	$(CCX) $(CFLAGS) vfd.cpp

 VFD_16LF01UA3.o:
	$(CCX) $(CFLAGS)  VFD_16LF01UA3.cpp

install:
	$(CP) $(appname) $(BININST)/$(appname)
	$(CP) vfd_display.sh $(BININST)/
	$(CP) vfd.service $(SYSTEMDINST)/
	$(LN) $(SYSTEMDINST)/vfd.service $(SYSTEMDENABLE)/vfd.service 

uninstall:

	$(RM) /usr/bin/vfd
	$(RM) /usr/bin/vfd_display.sh
	$(RM) /lib/systemd/system/vfd.service
	$(RM) /etc/systemd/system/multi-user.target.wants/vfd.service 
