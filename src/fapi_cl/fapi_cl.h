#define SELF_IP "192.168.130.81"
#define SELF_PORT 6789
#define UE_SIM_IP "172.24.100.56"
#define UE_SIM_PORT 9876

#define MAX_BUF_LEN 110000

#define EVT_RECP_REQUEST 4
#define EVT_DATA_REQUEST 12
#define EVT_CNTL_REQUEST 15

typedef struct clCb
{
   bool      configured;
   uint32_t  sockfd;
   struct    sockaddr_in srcAddr;
   struct    sockaddr_in dstAddr;
}ClCb;

ClCb clCb;

void sendMsgToUeSim(uint8_t event, Data *msg, uint16_t msgLen);
