#!/bin/bash

echo "hello world"

LED_PATH="/sys/class/leds/beaglebone:green:usr3"

function removeTrigger
{
    echo "none" >> $LED_PATH/trigger
}
function setTrigger
{
    echo "timer" >> $LED_PATH/trigger
}

if [ $# != 1 ]; then
    echo "Error: wrong usage, follow below syntax:"
    echo -e "./BashLED.sh followed by one of these cmd...\n-on\n-off\n-flash\n-status\ne.g. ./BashLED.sh on"
    exit 2
fi

if [ $1 == "on" ]; then
    echo "turn led on"
    removeTrigger
    echo 1 >> $LED_PATH/brightness
elif [ $1 == "off" ]; then
    echo "turn led off"
    removeTrigger
    echo 0 >> $LED_PATH/brightness
elif [ $1 == "flash" ]; then
    echo "flash led"
    removeTrigger
    setTrigger
    echo "50" >> $LED_PATH/delay_on
    echo "50" >> $LED_PATH/delay_off
elif [ $1 == "status" ]; then
    echo "led is triggered as:"
    cat $LED_PATH/trigger
fi
