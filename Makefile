CC=g++
CFLAGS=-c -Wall
LFLAGS=-lpigpio
CP=/bin/cp
LN=/bin/ln -sf
RM=/bin/rm -f
all: prog

prog: vfd.o
	$(CC) $(LFLAGS) vfd.o -o vfd

clean:
	$(RM) *.o
	$(RM) vfd

vfd.o:
	$(CC) $(CFLAGS) vfd.cpp

install:
	$(CP) vfd /usr/bin/vfd
	$(CP) vfd_display.sh /usr/bin/
	$(CP) vfd.service /lib/systemd/system/
	$(LN) /usr/lib/systemd/system/vfd.service /etc/systemd/system/multi-user.target.wants/vfd.service 

uninstall:

	$(RM) /usr/bin/vfd
	$(RM) /usr/bin/vfd_display.sh
	$(RM) /lib/systemd/system/vfd.service
	$(RM) /etc/systemd/system/multi-user.target.wants/vfd.service 
