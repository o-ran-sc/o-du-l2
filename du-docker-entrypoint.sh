export DU_LOCAL=`ifconfig eth0 | grep 'inet ' | cut -f2 | awk '{ print $2}'`
echo DU_F1C IP address is "$DU_LOCAL"

CU_IP=`nslookup ${CU_ADDRESS} | grep Address | awk 'NR==2' | cut -d: -f2 | awk '{print $1}'`
if [ -z "$CU_IP" ]
then
        echo "DU not able to fetch CU Service IP address"
        echo "DU Stopped......!"
        exit
else
        echo CU Service IP address is "$CU_IP"
fi
CU_SVC_IP="$CU_IP"

RIC_IP=`nslookup ${RIC_ADDRESS} | grep Address | awk 'NR==2' | cut -d: -f2 | awk '{print $1}'`
if [ -z "$RIC_IP" ]
then
        echo "DU not able to fetch RIC Service IP address"
        echo "DU Stopped......!"
        exit
else
        echo RIC Service IP address is "$RIC_IP"
fi
RIC_SVC_IP="$RIC_IP"

sed -i "s/192.168.130.82/$CU_IP/g" l2/build/config/fdd_odu_config.xml l2/build/config/tdd_odu_config.xml 
sed -i "s/192.168.130.80/$RIC_IP/g" l2/build/config/fdd_odu_config.xml l2/build/config/tdd_odu_config.xml 
sed -i "s/192.168.130.81/$DU_LOCAL/g" l2/build/config/fdd_odu_config.xml l2/build/config/tdd_odu_config.xml 
cd l2/build/odu
make clean_odu odu MACHINE=BIT64 MODE=FDD


