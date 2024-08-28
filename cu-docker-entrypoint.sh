export CU_LOCAL=`ifconfig eth0 | grep 'inet ' | cut -f2 | awk '{ print $2}'`
echo CU_LOCAL IP address is "$CU_LOCAL"


DU_IP=`nslookup ${DU_ADDRESS} | grep Address | awk 'NR==2' | cut -d: -f2 | awk '{print $1}'`
if [ -z "$DU_IP" ]
then
        echo "CU not able to fetch DU Service IP address"
        echo "CU Stopped......!"
        exit
else
        echo DU Service IP address is "$DU_IP"
fi
DU_SVC_IP="$DU_IP"

sed -i "s/192.168.130.82/$CU_LOCAL/g" l2/src/cu_stub/cu_stub.h 
sed -i "s/192.168.130.81/$DU_IP/g" l2/src/cu_stub/cu_stub.h
cd /root/l2/build/odu/
sed -i "s/UCONTAINERIZE/DCONTAINERIZE/g" makefile
make clean_cu cu_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD


