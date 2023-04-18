#!/bin/bash
STARTIP="pcfeib430-142-"
ENDIP=".vsb.cz"
STARTNUM=69
ENDNUM=85

for i in `seq $STARTNUM $ENDNUM`
do
    #check if the machine is up
    ping -c 1 $STARTIP$i$ENDIP

    #if ping ok, then connect
    if [ $? -eq 0 ]; then
        echo "Machine $STARTIP$i$ENDIP is up"
        ssh min0150@$STARTIP$i$ENDIP
        # ask if you want to connect to another machine
        echo "Do you want to connect to another machine? (y/n)"
        read answer
        if [ $answer = "n" ]; then
            break
        fi
        
    else
        echo "Machine $STARTIP$i$ENDIP is down"
    fi
done