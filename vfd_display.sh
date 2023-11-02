#!/bin/bash

VFD_PROG=/usr/bin/vfd

$VFD_PROG -ic
$VFD_PROG -r "Hello, this is vfd on Raspberry Pi program"
while IFS= read -r i
do
	$VFD_PROG -cr "IP: $i on ETH"
done <<< $(/sbin/ip -o -4 addr list | awk '{print $4 " - " $2}' | sed 's/\/[0-9]\+//')
$VFD_PROG -cs "   GOOD DAY"
sleep 1
$VFD_PROG -c
sleep 1
$VFD_PROG -cs "   GOOD DAY"
sleep 1
$VFD_PROG -cms " TO JEST TO !!!!"
while true
do
	while IFS= read -r line;
	do
		$VFD_PROG -cr "$line"
	done <<< $(fortune)
done


