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

if [ "$1" == "load-yang" ]
then
   sysrepoctl -i ../yang/o-ran-sc-odu-alarm-v1.yang
   sysrepoctl -i ../yang/o-ran-sc-du-hello-world.yang
   sysrepoctl -i ../yang/o-ran-sc-odu-interface-v1.yang
   sysrepocfg --import=../config/startup_config.xml --datastore running --module  o-ran-sc-odu-interface-v1
fi

if [ "$1" == "cleanup" ]
then
   kill -9 `pidof netopeer2-server`
   cd ../netconf/sysrepo/build/
   make sr_clean
   cd ../../Netopeer2/build/
   make install
   cd ../../../scripts/


   sysrepoctl -i ../yang/o-ran-sc-odu-alarm-v1.yang
   sysrepoctl -i ../yang/o-ran-sc-du-hello-world.yang
   sysrepoctl -i ../yang/o-ran-sc-odu-interface-v1.yang
   sysrepocfg --import=../config/startup_config.xml --datastore running --module  o-ran-sc-odu-interface-v1
fi
