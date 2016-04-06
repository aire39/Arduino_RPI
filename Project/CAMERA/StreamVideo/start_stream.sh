#! /bin/bash

clear

raspivid -n -t 0 -rot 0 -w 960 -h 720 -fps 30 -b 6000000 -o - | gst-launch-1.0 -e -vvvv fdsrc ! h264parse ! rtph264pay pt=96 config-interval=5 ! udpsink host=192.168.29.101 port= 5000
