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
all: prog

.PHONY: all prog vfd.o lib clean install uninstall

$(appname): vfd.o
	$(CC) $(LDLIBS) vfd.o -o vfd

clean:
	$(RM) *.o $(appname)
	$(RM) .PHONY: all lib depend clean dist-clean install uninstall

vfd.o:
	$(CC) $(CFLAGS) vfd.cpp

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
