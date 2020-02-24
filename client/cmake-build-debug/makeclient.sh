#!/bin/bash

echo make client ...
if [ -z $1];then
	echo please input how many clients will create
	exit 1
fi

i=0
while [ $i -lt $1 ]
do
	./client -p 9100 -l ../temp.txt -m "PI000$i" &
	let i+=1
done

