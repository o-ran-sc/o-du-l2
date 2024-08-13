#export pod_name=$(kubectl get pods | grep du- | cut -f2 | awk '{ print $1}')
#export DU_IP_ADD=$(kubectl exec -it $pod_name -- ifconfig eth0 | grep 'inet ' | cut -f2 | awk '{ print $2}')
#echo DU_IP="$DU_IP_ADD"

#export DU_IP=`nslookup  | grep 'inet ' | cut -f2 | awk '{ print $2}'`
#echo DU IP address is "$DU_IP"

export CU_LOCAL=`ifconfig eth0 | grep 'inet ' | cut -f2 | awk '{ print $2}'`
echo CU_LOCAL IP address is "$CU_LOCAL"


#CU_IP=`nslookup ${CU_ADDRESS} | grep Address | awk 'NR==2' | cut -d: -f2 | awk '{print $1}'`
#while[ -z "$CU_IP" ]
#do
#        echo "CU not able to fetch CU Service IP address"
#        echo "CU Stopped......!"
#        CU_IP=`nslookup ${CU_ADDRESS} | grep Address | awk 'NR==2' | cut -d: -f2 | awk '{print $1}'`
#        echo CU Service IP address is "$CU_IP"
#        sleep 2
#done
#CU_SVC_IP="$CU_IP"


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
make clean_cu cu_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD
#cd bin
#./cu_stub/cu_stub
#CU_IP=`nslookup CU_ADDRESS | grep Address | awk 'NR==2' | cut -d: -f2 | awk '{print $1}'`
#echo CU Service IP address is "$CU_IP"


