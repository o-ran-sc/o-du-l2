#!/bin/bash

if [ "$1" == "" ]
then
   echo Usage : netopeer.sh [start] [stop]
fi

if [ "$1" == "start" ]
then
   netopeer2-server -d -v2 > /var/log/netopeer2-server.log 2>&1 &
fi

if [ "$1" == "stop" ]
then
   kill -9 `pidof netopeer2-server`
fi
