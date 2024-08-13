#export pod_name=$(kubectl get pods | grep du- | cut -f2 | awk '{ print $1}')
#export DU_IP_ADD=$(kubectl exec -it $pod_name -- ifconfig eth0 | grep 'inet ' | cut -f2 | awk '{ print $2}')
#echo DU_IP="$DU_IP_ADD"

#export DU_IP=`nslookup  | grep 'inet ' | cut -f2 | awk '{ print $2}'`
#echo DU IP address is "$DU_IP"

export RIC_LOCAL=`ifconfig eth0 | grep 'inet ' | cut -f2 | awk '{ print $2}'`
echo RIC_LOCAL IP address is "$RIC_LOCAL"


#RIC_IP=`nslookup ${RIC_ADDRESS} | grep Address | awk 'NR==2' | cut -d: -f2 | awk '{print $1}'`
#while[ -z "$RIC_IP" ]
#do
#        echo "RIC not able to fetch RIC Service IP address"
#        echo "RIC Stopped......!"
#        RIC_IP=`nslookup ${RIC_ADDRESS} | grep Address | awk 'NR==2' | cut -d: -f2 | awk '{print $1}'`
#        echo RIC Service IP address is "$RIC_IP"
#        sleep 2
#done
#RIC_SVC_IP="$RIC_IP"


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
make clean_ric ric_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD
#cd bin
#./ric_stub/ric_stub
#RIC_IP=`nslookup RIC_ADDRESS | grep Address | awk 'NR==2' | cut -d: -f2 | awk '{print $1}'`
#echo RIC Service IP address is "$RIC_IP"


