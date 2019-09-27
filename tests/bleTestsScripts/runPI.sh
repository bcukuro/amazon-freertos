#!/bin/sh

scp * pi@192.168.21.193: 
ssh -t -t 192.168.21.193 -l pi << 'ENDSSH'
sudo rm -rf "/var/lib/bluetooth/*"
sudo hciconfig hci0 reset
python test1.py
sleep 1
ENDSSH

