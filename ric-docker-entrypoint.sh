export RIC_LOCAL=`ifconfig eth0 | grep 'inet ' | cut -f2 | awk '{ print $2}'`
echo RIC_LOCAL IP address is "$RIC_LOCAL"

DU_IP=`nslookup ${DU_ADDRESS} | grep Address | awk 'NR==2' | cut -d: -f2 | awk '{print $1}'`
if [ -z "$DU_IP" ]
then
        echo "RIC not able to fetch DU Service IP address"
        echo "RIC Stopped......!"
        exit
else
        echo DU Service IP address is "$DU_IP"
fi
DU_SVC_IP="$DU_IP"

sed -i "s/192.168.130.80/$RIC_LOCAL/g" l2/src/ric_stub/ric_stub.h 
sed -i "s/192.168.130.81/$DU_IP/g" l2/src/ric_stub/ric_stub.h
cd /root/l2/build/odu/
sed -i "s/UCONTAINERIZE/DCONTAINERIZE/g" makefile
make clean_ric ric_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD


