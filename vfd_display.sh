#!/bin/bash

VFD_PROG=./vfd

$VFD_PROG -ic
$VFD_PROG -r "Hello, this is vfd on Raspberry Pi program"
for i in "$(/sbin/ip -o -4 addr list | awk '{print $4 " - " $2}' | sed 's/\/[0-9]\+//')"
do
	$VFD_PROG -cr "IP: $i on ETH"
done
$VFD_PROG -cs "   GOOD DAY"
sleep 1
$VFD_PROG -c
sleep 1
$VFD_PROG -cs "      GOOD DAY"
sleep 1
$VFD_PROG -cms " TO JEST TO !!!!"
while true
do
	$VFD_PROG -cr "$(fortune)"
done
