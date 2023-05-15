/********************************************************************16**

            (c) Copyright 2012 by RadiSys Corporation. All rights reserved.

     This software is confidential and proprietary to RadiSys Corporation.
     No part of this software may be reproduced, stored, transmitted, 
     disclosed or used in any form or by any means other than as expressly
     provided by the written Software License Agreement between Radisys 
    and its licensee.

     Radisys warrants that for a period, as provided by the written
     Software License Agreement between Radisys and its licensee, this
     software will perform substantially to Radisys specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  Radisys also warrants 
     that has the corporate authority to enter into and perform under the 
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

    RADISYS MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
    MATERIALS.

     IN NO EVENT SHALL RADISYS BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
    OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend:

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
    nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between Radisys and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact Radisys at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    RadiSys Corporation
                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388
                    Email: support@trillium.com
                    Web: http://www.radisys.com 
 
*********************************************************************17*/


/********************************************************************20**

     Name:     Multi-threaded System Services - Solaris

     Type:     C source file

     Desc:     C source code for the MTSS-Solaris implementation of
               System Services.

     File:     ss_dpdk.c

     Sid:      pt_ss.c@@/main/TeNB_Main_BR/11 - Mon Aug 11 16:44:04 2014

     Prg:      kp

*********************************************************************21*/
#ifdef DPDK_ENABLE
#include "envopt.h"
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
//#include "cm_os.h"
#include "ss_dpdk.h"

#include "gen.x"           /* general layer */
#include "ssi.x"

#include "tfu.h"
#include <stdbool.h>
//#include "cm_os.x"
#include "ss_dpdk.x"
//#include "ue_log.h"
//#include "ue.h"
#include <rte_arp.h>
#include <rte_hash.h>
#include "cm_inet.h"
#include "cm_inet.x"
#include "fapi_cl.h"
#include "fapi_cl_utils.h"
#include "common_def.h"
#ifdef FAST_CRYPTO_ENABLED
S8   pci_add[MAX_PCI_ADDRESS_SIZE];
#include "ngp_fast_crypto.h"
#include "ngp_data_pool.h"
#endif
#include "cm_lib.x"

#ifdef FAPI_SUPPORT_10_03
#include "fapi_phy_sim_l2_msg_hndlr.h"
#include "uesim_fapi_phy_types.h"
#include "fapi.h"
#endif
#ifdef UESIM_TIME_PROFILE
///Added for test purpose
#include <iostream>
#include <chrono>
#include <unistd.h>
std::chrono::time_point<std::chrono::high_resolution_clock> g1;
std::chrono::time_point<std::chrono::high_resolution_clock> t1;
std::chrono::time_point<std::chrono::high_resolution_clock> t2;
std::chrono::time_point<std::chrono::high_resolution_clock> t3;
using namespace std;
using namespace std::chrono;
#endif

extern  sem_t ysPalDldataSigSem;
bool FAST_CRYPTO_AS_SECURITY = false;
bool FAST_CRYPTO_THREAD_ENABLE = false;
struct rte_mempool *bbuio_buf_pool = NULL;
struct rte_mempool *p_rte_direct_mem_pool;
struct rte_mempool *p_rte_indirect_mem_pool;

typedef struct port_statistics_s {
   uint64_t tx_packets;
   uint64_t rx_packets;
   uint64_t dropped_packets;
   uint64_t tx_bytes;
   uint64_t rx_bytes;
   uint64_t dropped_bytes;
}port_statistics_t;

uint16_t nb_rxd = RTE_TEST_RX_DESC_DEFAULT;
static uint16_t nb_txd = RTE_TEST_TX_DESC_DEFAULT;

/* ethernet addresses of ports */
/* rx config structure */
static struct rte_eth_rxconf rx_cfg  = {
   .rx_thresh = {
      .pthresh = RX_PTHRESH,
      .hthresh = RX_HTHRESH,
      .wthresh = RX_WTHRESH,
   },
   .rx_free_thresh = 32,
   .rx_drop_en = 0,
   .rx_deferred_start = 0,
   //.rx_nseg = 0,
   .offloads = 0
};


/* tx config structure */
static struct rte_eth_txconf tx_cfg  = {
   .tx_thresh = {
      .pthresh = TX_PTHRESH,
      .hthresh = TX_HTHRESH,
      .wthresh = TX_WTHRESH,
   },
   .tx_rs_thresh = 32,
   .tx_free_thresh = 32,
   //.txq_flags = 0,
   .tx_deferred_start = 0,
   .offloads = 0
};

static enum rte_eth_nb_pools num_pools = ETH_16_POOLS;
const uint16_t vlan_tags[] = {
   0,  1,  2,  3,  4,  5,  6,  7,
   8,  9,  10, 11, 12, 13, 14, 15,
   16, 17, 18, 19, 20, 21, 22, 23,
   24, 25, 26, 27, 28, 29, 30, 31
};

/* port config structure */
static const struct rte_eth_conf portcfg  = {
   .link_speeds = 0,
   .rxmode  = {
      .mq_mode = ETH_MQ_RX_NONE,
      //.max_rx_pkt_len = 0x2600,
      .max_lro_pkt_size = 0,
      .split_hdr_size = 0,
      //.offloads = DEV_RX_OFFLOAD_JUMBO_FRAME,
      //.header_split = 0,
      //.hw_ip_checksum = 0,
      //.hw_vlan_filter = 0,
      //.hw_vlan_strip = 0,
      //.hw_vlan_extend = 0,
      //.jumbo_frame = 1,
      //.hw_strip_crc = 0,
   },
   .txmode = {
      .mq_mode = ETH_MQ_TX_NONE,
      .offloads = DEV_TX_OFFLOAD_MULTI_SEGS
   },
   .lpbk_mode = 0,
   .rx_adv_conf = {
      .rss_conf = {
         .rss_key = NULL,
         .rss_key_len = 0,
         //.rss_hf = ETH_RSS_IP,
      },
      .vmdq_dcb_conf = {
         .nb_queue_pools = ETH_16_POOLS,
         .enable_default_pool = 1,
         .default_pool = 0,
         .nb_pool_maps = 0,
         .pool_map = {{0, 0}},
         .dcb_tc = {0},
      },
   }
};


typedef struct tx_mbuf_table_s {
   uint16_t nb_tx_mbufs;
   struct rte_mbuf *tx_mbufs[TX_MBUF_TABLE_SIZE];
}tx_mbuf_table_t;

typedef struct pci_eth_dev_s {
   uint16_t  port_id;
   int8_t   pci_address[MAX_PCI_ADDRESS_SIZE];
   uint8_t  nb_rx_queues;
   uint8_t  nb_tx_queues;
   uint16_t nb_rx_ring_dsc;
   uint16_t nb_tx_ring_dsc;
   struct   rte_ip_frag_tbl *frag_tbl;
   struct   rte_ip_frag_death_row death_row;
   struct   rte_eth_conf port_config;
   struct   rte_mbuf *rx_pkts[MAX_PKT_BURST_RX];
   uint8_t  rx_queue_id[MAX_RX_QUEUES_PER_PORT];
   uint8_t  tx_queue_id[MAX_TX_QUEUES_PER_PORT];
   struct   rte_ether_addr src_eth_addr;
   struct   rte_ether_addr dst_eth_addr;
   struct   rte_ether_hdr tgen_addr;
   tx_mbuf_table_t tx_mbuf_tbl;
   port_statistics_t port_statistics;
} pci_eth_dev_t;

typedef enum pci_addr_port_val
{
   L2_FWD_PCI_ADDR,
   NG_PCI_ADDR,
   MAX_PCI_ADDR
} pci_addr_port_val_e;

typedef enum dpdk_mem_pool_id
{
   RX_L2_FWD_MEM_POOL,
   RX_NG_MEM_POOL,
   RX_MAX_MEM_POOL
} dpdk_mem_pool_id_e;

void *dpdk_mem_pools[RX_MAX_MEM_POOL];
pci_eth_dev_t p_pci_eth_dev[MAX_PCI_ADDR];

typedef struct thread_config
{
   uint16_t n;
   uint16_t eal_tid;
   uint16_t rx_tid;
   uint16_t tgen_rx_tid;
   uint16_t tti_tmr_tid;
}thread_config_t;

typedef struct pci_config
{
   S8 pci_addr_fronthaul[MAX_PCI_ADDRESS_SIZE];
   S8 pci_addr_backhaul[MAX_PCI_ADDRESS_SIZE];
   S8 dst_mac_addr_fh[MAX_MAC_ADDRESS_SIZE];
   S8 tgen_src_mac_addr[MAX_MAC_ADDRESS_SIZE];
   S8 tgen_dst_mac_addr[MAX_MAC_ADDRESS_SIZE];
   uint32_t uesim_to_vc_intf_ip;
}pci_config_t;

typedef struct seg_info
{
   uint16_t is_jumbo_payload_en;
   uint16_t max_seg_size;
} seg_info_t;

typedef struct udp_pkt
{
   uint16_t        uesim_udp_port;
   uint16_t        gnb_udp_port;
   S8              uesim_udp_addr[MAX_IP_ADDRESS_SIZE];
   S8              gnb_udp_addr[MAX_IP_ADDRESS_SIZE];
} udp_pkt_t;

#define MAX_FILENAME_SZ 16
#define ARP_PKT_SIZE 64
typedef struct dpdk_config 
{
   uint16_t        is_fronthaul_en;
   uint16_t        is_backhaul_en;
   uint16_t        is_blacklist_en;
   S8              *pci_addr_to_blacklist;
   S8              hugepage_conf_file[MAX_FILENAME_SZ];
   thread_config_t thread_conf;
   pci_config_t    pci_conf;
   seg_info_t      seg_info;
   uint16_t        fh_vlan_tag_pres;
   uint16_t        mh_vlan_tag_pres;
   udp_pkt_t       udp_packet;
   /* Fast Crypto Params start */
   uint16_t        enable_crypto_init;
   uint16_t        max_num_crypto_session;
   uint16_t        num_fast_crypto_port;
   S8              pci_address[MAX_PCI_ADDRESS_SIZE];
   uint16_t        num_queues;
   uint16_t        crypto_thread_enable;             
   /* Fast Crypto Params end */
   uint16_t        du_mac_address_validation;
} dpdk_config_t;

dpdk_config_t gCfg;

static struct rte_hash *ip2mac_table;

#define printMacAddres(eth_addr)                        \
{                                                       \
   printf("MAC: [%02X:%02X:%02X:%02X:%02X:%02X]\n",     \
         eth_addr->addr_bytes[0],                       \
         eth_addr->addr_bytes[1],                       \
         eth_addr->addr_bytes[2],                       \
         eth_addr->addr_bytes[3],                       \
         eth_addr->addr_bytes[4],                       \
         eth_addr->addr_bytes[5]);                      \
}

#define printMacAddres2(eth_addr)                       \
{                                                       \
   printf("MAC: [%02X:%02X:%02X:%02X:%02X:%02X]\n",     \
         eth_addr.addr_bytes[0],                        \
         eth_addr.addr_bytes[1],                        \
         eth_addr.addr_bytes[2],                        \
         eth_addr.addr_bytes[3],                        \
         eth_addr.addr_bytes[4],                        \
         eth_addr.addr_bytes[5]);                       \
}

 int16_t dpdk_config_init (dpdk_config_t *conf); 
 int16_t send_packet (struct rte_mbuf **pkts_burst, uint16_t portid, uint16_t n);
 int32_t dpdk_eal_init (void);
 int32_t dpdk_rx_thread (uint16_t fh_vlan_tag_pres);
#ifdef NFAPI_SUPPORT
 int32_t dpdk_tti_formation_thread(__attribute__(unused) void *dummy);
#endif
 int16_t clone_pci_eth_dev (pci_eth_dev_t *p_eth_dev, const S8 *pci_addr_str);
  void print_stats  (pci_eth_dev_t *pci_eth_dev);
#ifdef EXT_TGEN_ENABLE
 void handle_ip_packet  (pci_eth_dev_t *pci_eth_dev,struct rte_mbuf *mBuf);
 void deliver_ip_packet  (pci_eth_dev_t *pci_eth_dev, uint8_t j, uint64_t tms);
 void eth_recv  (pci_eth_dev_t *pci_eth_dev, uint64_t tms);
 int32_t ng_main  (void *args);

 void hash_init  (void);
 void handle_recvd_arp_message  (pci_eth_dev_t *pci_eth_dev,struct rte_mbuf *mBuf);
 bool find_arp_entry  (uint32_t ip_addr, struct rte_ether_addr *eth_addr);
 bool send_arp_req  (uint16_t eth_port_id, uint32_t dst_ip_addr, uint32_t src_ip_addr);
#endif

extern volatile Bool gTtiReady;
extern volatile uint32_t  YS_TTI_TMR_VAL_CFG;

S8 dpdk_config_params[][DPDK_MAX_CFG_PARAM_LEN] = {
   "hugepage_config_file",
   "max_dpdk_core",
   "enable_fronthaul",
   "enable_backhaul",
   "eal_tid",
   "fronthaul_rx_tid",
   "backhaul_tx_rx_tid",
   "tti_tmr_tid",
   "fh_vlan_tag_pres",
   "mh_vlan_tag_pres",
   "pci_address_fronthaul",
   "pci_address_backhaul",
   "dst_mac_address_fronthaul", 
   "tgen_src_mac_address_backhaul",
   "tgen_dst_mac_address_backhaul",
   "enable_jumbo_payload",
   "max_segment_size",
   "uesim_to_vc_intf_ip",
   "enable_crypto_init",
   "max_num_crypto_session",
   "num_fast_crypto_port",
   "pci_address",
   "num_queues",
   "crypto_thread_enable",
   "uesim_udp_port",
   "gnb_udp_port",
   "uesim_udp_addr",
   "gnb_udp_addr"
   ,"du_mac_address_validation"
};
S32 osStripBlanks(S8 *rawStr)
{
   S8 *rawPtr;
   S8 *outPtr;
   S8 outStr[256];


   strcpy(outStr, (const S8*)"\0");
   rawPtr = rawStr;
   outPtr = outStr;

   while (((isspace ((int)*rawPtr)) || (iscntrl ((int)*rawPtr)))
            && (*rawPtr != '\0'))
      rawPtr++;

   while (*rawPtr != '\0')
      *outPtr++ = *rawPtr++;

   while (((isspace (*(outPtr-1))) || (iscntrl (*(outPtr-1)))) &&
          (outPtr >= outStr))
      outPtr--;

   *outPtr = '\0';

   strcpy(rawStr, (const S8*)outStr);

   return (strlen(rawStr));
} 
 S8 *osGetWord( S8 *line,   S8 *word      )
{
   REG1 S32 i;
   REG2 S8 *lPtr;
   REG3 S8 *wPtr;

  // TRC2(osGetWord)

   lPtr = line;                 /* line pointer */
   wPtr = word;                 /* word pointer */

   while ( lPtr && isspace((int)*lPtr) ) /* skip leading white space */
      lPtr++;

   /*
    * make sure we're not at end of line
    */
   if ( !lPtr || *lPtr == '\n' || *lPtr == '\0' )
   {
      *word = '\0';
      RETVALUE(0);
   }

   if (*lPtr == '"') /* handle quoted words */
   {
      lPtr++;        /* munch first quote */
      i = 0;
      while ( *lPtr !=  '"')
      {
         if ( *lPtr == '\n' || *lPtr == '\0')
         {
            *word = '\0';
            RETVALUE(0);
         }
         *wPtr++ = *lPtr++;
         i++;
      }
      lPtr++;        /* munch last quote */
   }
   else /* handle unquoted words */
   {
      i = 0;
      while ( !isspace((int)*lPtr) && *lPtr != '\n' && *lPtr != '\0')
      {
         *wPtr++ = *lPtr++;
         i++;
      }
   }
   *wPtr = '\0';                /* null terminate */
   RETVALUE(lPtr);              /* return pointer to remainder of line */
} /* end of osGetWord */

 int16_t dpdk_config_init (dpdk_config_t *conf)
{
   FILE *fp;
   uint8_t     lineLen;
   S8     line[DPDK_MAX_CFG_FILE_LINE_LEN];
   S8     *ret = ROK;
   S8     *lPtr;
   S8     word[DPDK_MAX_CFG_PARAM_LEN];
   Bool   isToken = TRUE;
   uint8_t     idx = 0;
   uint8_t     i = 0;
   uint8_t     numParams;
   uint8_t     lineNo;

   fp = fopen("dpdk_config.txt", "r+");
   if (fp == NULL)
   {
      printf("Unable to open the file\n");
      RETVALUE(RFAILED);
   }

   numParams = (uint8_t)(sizeof(dpdk_config_params)/DPDK_MAX_CFG_PARAM_LEN);

   lineNo = 1;

   while (1)
   {
      memset ((uint8_t *)line, 0, DPDK_MAX_CFG_FILE_LINE_LEN);

      ret = fgets(line, DPDK_MAX_CFG_FILE_LINE_LEN, fp);

      if (ret == (S8 *)-1 || ret == NULLD)
      {
         break;
      }

      if (!(lineLen = (uint8_t)(osStripBlanks(line))))
      {
         lineNo++;
         continue;
      }

      if (*line == '#')
      {
         lineNo++;
         continue;
      }

      lPtr = line;
      i = 0;
      isToken = TRUE;

      while ((lPtr = osGetWord (lPtr, word)) != NULLP)
      {
         if (word[0] == '#')
            break;

         if (isToken)
         {
            for (idx = 0 ; idx < numParams ; idx++)
            {
               if (!strcmp(word, dpdk_config_params[idx]))
               {
                  isToken ^= TRUE;
                  break;
               }

               if (idx == numParams)
               {
                  printf("[FAILED]: dpdk_config.txt: Invalid Configuration Parameter\n");
                  RETVALUE(RFAILED);
               }
            }
         }
         else
         {
            switch(idx)
            {
               case 0:
                  {
                     strcpy((S8 *)conf->hugepage_conf_file, word);
                     break;
                  }
               case 1:
                  {
                     conf->thread_conf.n = (uint16_t)strtol(word, NULL, 10);
                     break;
                  }
               case 2:
                  {
                     conf->is_fronthaul_en = (uint16_t)strtol(word, NULL, 10);
                     break;
                  }
               case 3:
                  {
                     conf->is_backhaul_en = (uint16_t)strtol(word, NULL, 10);
                     break;
                  }
               case 4:
                  {
                     conf->thread_conf.eal_tid = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 5:
                  {
                     conf->thread_conf.rx_tid = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 6:
                  {
                     conf->thread_conf.tgen_rx_tid = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 7:
                  {
                     conf->thread_conf.tti_tmr_tid = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 8:
                  {
                     conf->fh_vlan_tag_pres = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 9:
                  {
                     conf->mh_vlan_tag_pres = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 10:
                  {
                     strcpy((S8*)conf->pci_conf.pci_addr_fronthaul, word);
                     break;
                  }
               case 11:
                  {
                     strcpy((S8*)conf->pci_conf.pci_addr_backhaul, word);
                     break;
                  }
               case 12:
                  {
                     conf->pci_conf.dst_mac_addr_fh[i++] = (uint8_t)strtol(word, NULL, 16);
                     isToken ^= TRUE;
                     break;
                  }
               case 13:
                  {
                     conf->pci_conf.tgen_src_mac_addr[i++] = (uint8_t)strtol(word, NULL, 16);
                     isToken ^= TRUE;
                     break;
                  }
               case 14:
                  {
                     conf->pci_conf.tgen_dst_mac_addr[i++] = (uint8_t)strtol(word, NULL, 16);
                     isToken ^= TRUE;
                     break;
                  }
               case 15:
                  {
                     conf->seg_info.is_jumbo_payload_en = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 16:
                  {
                     conf->seg_info.max_seg_size = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 17: //uesim_to_vc_intf_ip
                  {
                     cmInetAddr(word, &conf->pci_conf.uesim_to_vc_intf_ip);
                     //conf->pci_conf.uesim_to_vc_intf_ip = htonl(conf->pci_conf.uesim_to_vc_intf_ip);
                     break;
                  }
               case 18:
                  {
                     conf->enable_crypto_init = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 19:
                  {
                     conf->max_num_crypto_session = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 20:
                  {
                     conf->num_fast_crypto_port = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 21:
                  {
                     strcpy((S8*)conf->pci_address, word);
                     break;
                  }
               case 22:
                  {
                     conf->num_queues = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 23:
                  {
                     conf->crypto_thread_enable = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 24: //uesim udp port for NFAPI P7 communication
                  
                  {
                     conf->udp_packet.uesim_udp_port = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 25: //gnb udp port for NFAPI P7 communication
                  {
                     conf->udp_packet.gnb_udp_port = (uint16_t) strtol (word, NULL, 10);
                     break;
                  }
               case 26: //uesim udp address for NFAPI P7 communication
                  {
                     strcpy((S8 *)conf->udp_packet.uesim_udp_addr, word);
                     break;
                  }
               case 27: //gnb udp address for NFAPI P7 communication
                  {
                     strcpy((S8 *)conf->udp_packet.gnb_udp_addr, word);
                     break;
                  }
               case 28:
                  {
                     conf->du_mac_address_validation = (uint16_t) strtol (word, NULL, 10);
                     break;
                  
                  }

               default:
                  {
                     break;
                  }
            }
            isToken ^= TRUE;
         }
         memset ((uint8_t *)word, 0, DPDK_MAX_CFG_PARAM_LEN);
      }
   }

//Removing the blacklisting of ports, we can have whitelisting concept. Code to be completely removed
#if 0
   conf->is_blacklist_en = 1;
   if (conf->is_fronthaul_en && conf->is_backhaul_en)
   {
      conf->is_blacklist_en = 0;
   }
   else
   {
      conf->pci_addr_to_blacklist = (!conf->is_fronthaul_en)?
                                    conf->pci_conf.pci_addr_fronthaul:
                                    conf->pci_conf.pci_addr_backhaul;
   }
#endif

   printf("\nmax_dpdk_threads      : %d\n"
            "is_fronthaul_enable   : %d\n"
            "is_backhaul_enable    : %d\n"
/*            "is_blacklist_enable   : %d\n" */
            "pci_addr_to_blacklist : %s\n"
            "eal_thread_id         : %d\n"
            "rx_thread_id          : %d\n"
            "tgen_rx_tid           : %d\n"
            "tti_tmr_tid           : %d\n"
            "fh_vlan_tag_pres      : %d\n"
            "mh_vlan_tag_pres      : %d\n"
            "pci_addr_fronthaul    : %s\n"
            "pci_addr_backhaul     : %s\n"
            "max_seg_size          : %d\n"
            "du_mac_address_validation : %d\n"
            "uesim_udp_port        : %d\n"
            "gnb_udp_port          : %d\n"
            "uesim_udp_addr        : %s\n"
            "gnb_udp_addr          : %s\n"
            ,
            gCfg.thread_conf.n, gCfg.is_fronthaul_en, gCfg.is_backhaul_en, /*gCfg.is_blacklist_en,*/
            gCfg.pci_addr_to_blacklist, gCfg.thread_conf.eal_tid,
            gCfg.thread_conf.rx_tid,
            gCfg.thread_conf.tgen_rx_tid, gCfg.thread_conf.tti_tmr_tid,
            gCfg.fh_vlan_tag_pres,gCfg.mh_vlan_tag_pres,gCfg.pci_conf.pci_addr_fronthaul,
            gCfg.pci_conf.pci_addr_backhaul, gCfg.seg_info.max_seg_size,
            gCfg.du_mac_address_validation,
            gCfg.udp_packet.uesim_udp_port, gCfg.udp_packet.gnb_udp_port,
            gCfg.udp_packet.uesim_udp_addr, gCfg.udp_packet.gnb_udp_addr
            );

            struct in_addr ip_addr;
            ip_addr.s_addr = gCfg.pci_conf.uesim_to_vc_intf_ip;
            //printf("uesim_to_vc_intf_ip: %s\n", inet_ntoa(ip_addr));
            printf("uesim_to_vc_intf_ip: %x\n", gCfg.pci_conf.uesim_to_vc_intf_ip);


   printf(  "tgen_src_mac_address  : %02x:%02x:%02x:%02x:%02x:%02x\n",
            gCfg.pci_conf.tgen_src_mac_addr[0], gCfg.pci_conf.tgen_src_mac_addr[1],
            gCfg.pci_conf.tgen_src_mac_addr[2], gCfg.pci_conf.tgen_src_mac_addr[3],
            gCfg.pci_conf.tgen_src_mac_addr[4], gCfg.pci_conf.tgen_src_mac_addr[5]);

   printf(  "tgen_dst_mac_address  : %02x:%02x:%02x:%02x:%02x:%02x\n",
            gCfg.pci_conf.tgen_dst_mac_addr[0], gCfg.pci_conf.tgen_dst_mac_addr[1],
            gCfg.pci_conf.tgen_dst_mac_addr[2], gCfg.pci_conf.tgen_dst_mac_addr[3],
            gCfg.pci_conf.tgen_dst_mac_addr[4], gCfg.pci_conf.tgen_dst_mac_addr[5]);
   if(gCfg.enable_crypto_init)
   {
   printf("enable_crypto_init  : %u\n",gCfg.enable_crypto_init);
   printf("max_num_crypto_session  : %u\n",gCfg.max_num_crypto_session);
   printf("pci_address  : %s\n",gCfg.pci_address);
   printf("crypto_thread_enable  : %u\n",gCfg.crypto_thread_enable);
   }
   fclose(fp);
   RETVALUE(ROK);
}

 void 
print_stats(pci_eth_dev_t *pci_eth_dev)
{
   struct rte_ether_addr ethAddr;

   const char clr[] = { 27, '[', '2', 'J', '\0' };
   const char topLeft[] = { 27, '[', '1', ';', '1', 'H','\0' };

   /* Clear screen and move to top left */
   printf("%s%s", clr, topLeft);

   rte_eth_macaddr_get(pci_eth_dev->port_id, &ethAddr);
   printf("\nPort statistics ====================================");

   printf("\n[NG_IO_APP] Port[%u] \nMAC-Addr[%02X:%02X:%02X:%02X:%02X:%02X]\n",
         (uint16_t) pci_eth_dev->port_id,
         ethAddr.addr_bytes[0],
         ethAddr.addr_bytes[1],
         ethAddr.addr_bytes[2],
         ethAddr.addr_bytes[3],
         ethAddr.addr_bytes[4],
         ethAddr.addr_bytes[5]);

   printf("\nStatistics for port %u ------------------------------"\
         "\nPackets sent: %lu"\
         "\nPackets received: %lu"\
         "\nPackets dropped: %lu"\
         "\nBytes sent: %lu"\
         "\nBytes received: %lu"\
         "\nBytes dropped: %lu",\
         pci_eth_dev->port_id,
         pci_eth_dev->port_statistics.tx_packets,
         pci_eth_dev->port_statistics.rx_packets,
         pci_eth_dev->port_statistics.dropped_packets,
         pci_eth_dev->port_statistics.tx_bytes,
         pci_eth_dev->port_statistics.rx_bytes,
         pci_eth_dev->port_statistics.dropped_bytes);

   printf("\n====================================================\n");

   pci_eth_dev->port_statistics.tx_packets = 0;
   pci_eth_dev->port_statistics.rx_packets = 0;
   pci_eth_dev->port_statistics.dropped_packets = 0;
   pci_eth_dev->port_statistics.tx_bytes = 0;
   pci_eth_dev->port_statistics.rx_bytes = 0;
   pci_eth_dev->port_statistics.dropped_bytes = 0;
}

 int16_t send_packet (struct rte_mbuf **pkts_burst, uint16_t portid, uint16_t n)
{
   uint16_t ret = 0;
   ret = rte_eth_tx_burst (portid, 0, pkts_burst, n);
   port_statistics_t port_statistics = p_pci_eth_dev[L2_FWD_PCI_ADDR].port_statistics;
   port_statistics.tx_packets += ret;
   if (unlikely (ret < n))
   {
      port_statistics.dropped_packets += (n - ret);
      do {
         rte_pktmbuf_free (pkts_burst[ret]);
      } while (++ret < n);
   }
   return 0;
}

int32_t dpdk_tx_send (Buffer *mBuf)
{
	struct rte_ether_hdr *eth;
	struct rte_mbuf *m;
	struct rte_ether_addr *src_eth_addr;
	struct rte_ether_addr *dst_eth_addr;
	struct rte_udp_hdr *udp_hdr;
	struct rte_ipv4_hdr *ipv4_hdr;
	static uint16_t packet_id = 0;

	uint16_t     portid;
	S8          *sendBuf;
	int32_t     ret = ROK;

	uint8_t      seg_cnt = 0;
	uint8_t      *tot_seg;
	uint8_t      *seg_no;
	uint8_t      idx = 0;

	MsgLen  msgLen;              /* message length */
	MsgLen  bufLen;
	MsgLen  *seg_len = 0;
	MsgLen  len = 0;
	MsgLen  start_idx = 0;

	portid = p_pci_eth_dev[L2_FWD_PCI_ADDR].port_id;
	src_eth_addr = &p_pci_eth_dev[L2_FWD_PCI_ADDR].src_eth_addr;
	//dst_eth_addr = &p_pci_eth_dev[NG_PCI_ADDR].dst_eth_addr;
	dst_eth_addr = &p_pci_eth_dev[L2_FWD_PCI_ADDR].dst_eth_addr;

	ret = SFndLenMsg(mBuf, &bufLen);
	if (ret != ROK)
	{
		RETVALUE(RFAILED);
	}

	uint16_t  seg_size = gCfg.seg_info.max_seg_size;
	seg_cnt = ((bufLen + (seg_size - 1))/seg_size);

	while (bufLen)
	{
		if (bufLen > seg_size)
		{
			len = seg_size;
			bufLen -= seg_size;
		}
		else
		{
			len = bufLen;
			bufLen = 0;
		}
		idx++;

		m = rte_pktmbuf_alloc (bbuio_buf_pool);

		sendBuf = rte_pktmbuf_append (m, len);
		ret = SCpyMsgFix(mBuf, start_idx, len, (Data *)sendBuf, &msgLen);

		if( (ret != ROK) || (msgLen != len)   )
		{
			/* Free the DPDK memory here */
			rte_pktmbuf_free (m);
			RETVALUE(RFAILED);
		}

		seg_len = (MsgLen *)rte_pktmbuf_prepend (m, sizeof(MsgLen));
		seg_no  = (uint8_t *) rte_pktmbuf_prepend (m, sizeof(uint8_t));
		tot_seg = (uint8_t *) rte_pktmbuf_prepend (m, sizeof(uint8_t));
		udp_hdr = (struct rte_udp_hdr*) rte_pktmbuf_prepend (m, sizeof(struct rte_udp_hdr));
		ipv4_hdr  = (struct rte_ipv4_hdr*) rte_pktmbuf_prepend (m, sizeof(struct rte_ipv4_hdr));
		eth     = (struct rte_ether_hdr *) rte_pktmbuf_prepend (m, sizeof(struct rte_ether_hdr));

		*seg_len = len;
		*tot_seg = seg_cnt;
		*seg_no  = idx;

		/* Fill udp header */
		udp_hdr->src_port    = htons(gCfg.udp_packet.uesim_udp_port);
		udp_hdr->dst_port    = htons(gCfg.udp_packet.gnb_udp_port);
		udp_hdr->dgram_len   = htons(sizeof(struct rte_udp_hdr)+DPDK_SEG_HDR_LEN+len);
		udp_hdr->dgram_cksum = 0;

		/* Fill ip header */
		if (65535 == packet_id)
		{
			packet_id = 0;
		}
		ipv4_hdr->src_addr = inet_addr((const char *)(gCfg.udp_packet.uesim_udp_addr));
		ipv4_hdr->dst_addr = inet_addr((const char *)(gCfg.udp_packet.gnb_udp_addr));
		ipv4_hdr->next_proto_id = (0x11);
		ipv4_hdr->time_to_live = 0x64;
		ipv4_hdr->type_of_service = 0;
#ifdef IP_FRAG_SUPPORT_ENABLED
		ipv4_hdr->fragment_offset = htons(0x0000);
#endif
#ifndef IP_FRAG_SUPPORT_ENABLED
		ipv4_hdr->fragment_offset = htons(0x4000);
#endif
		ipv4_hdr->version_ihl = 0x45;
		ipv4_hdr->packet_id = htons(++packet_id);
		ipv4_hdr->total_length = htons(sizeof(struct rte_ipv4_hdr) + sizeof(struct rte_udp_hdr) + DPDK_SEG_HDR_LEN + len);
		ipv4_hdr->hdr_checksum = rte_ipv4_cksum(ipv4_hdr);
		/* Fill ethernet header */
		rte_ether_addr_copy(src_eth_addr, &eth->s_addr);
		rte_ether_addr_copy(dst_eth_addr, &eth->d_addr);
		eth->ether_type = htons(ETHERNET_TYPE);

		send_packet(&m, portid, 1);
		start_idx += len;
	}

	RETVALUE(ROK);

}
 
int32_t dpdk_rx_thread(uint16_t fh_vlan_tag_pres)
{
	   uint8_t nb_rx = 0;
   Buffer *rcvBuf;
   struct rte_mbuf *rx_buff[MAX_PKT_BURST_RX];
   uint8_t j = 0;
   uint8_t vlan_tag_hdr;
   uint8_t portid = p_pci_eth_dev[L2_FWD_PCI_ADDR].port_id;
   port_statistics_t port_statistics = p_pci_eth_dev[L2_FWD_PCI_ADDR].port_statistics;

   MsgLen recvLen = 0;
   Data  *buf, bufToSend[MAX_BUF_LEN];
   Data  prmtv;

   uint8_t  indx = 1;

   Bool   dropPkt = FALSE;
   uint8_t     *seg_no;
   uint8_t     *tot_seg;
   MsgLen *seg_len;
   struct rte_ether_hdr *eth_hdr;
   struct rte_ipv4_hdr  *ipv4_hdr;
   struct rte_udp_hdr   *udp_hdr;

   Pst pst;
   pst.dstProcId = SFndProcId();
   pst.srcProcId = SFndProcId();
   pst.dstEnt = ENTFAPICL;
   pst.srcEnt = ENTFAPICL;
   pst.srcInst = FAPI_CL_INST_0;
   pst.dstInst = FAPI_CL_INST_0;
   pst.route = RTESPEC;
   pst.prior = PRIOR0;
   pst.region = CL_MEM_REGION;
   pst.pool = CL_POOL;
   pst.selector = ODU_SELECTOR_LWLC;
   uint8_t ret = 0;

   vlan_tag_hdr = gCfg.fh_vlan_tag_pres ? 4:0;
   while (1)
   {
      nb_rx = rte_eth_rx_burst(portid, 0, rx_buff, MAX_PKT_BURST_RX);

      /* Extract the ether header, Validate the Header */
      /* Get the Message Length */
      /* Process the message */
      port_statistics.rx_packets += nb_rx;
      for (j = 0 ; j < nb_rx ; j++)
      {
              recvLen = rte_pktmbuf_pkt_len(rx_buff[j]);
              eth_hdr = (struct rte_ether_hdr *) rte_pktmbuf_mtod_offset (rx_buff[j], struct rte_ether_hdr *, 0);
              if (eth_hdr->ether_type != rte_be_to_cpu_16(RTE_ETHER_TYPE_IPV4))
              {
                      rte_pktmbuf_free (rx_buff[j]);
                      continue;
        }
        ipv4_hdr = (struct rte_ipv4_hdr *) rte_pktmbuf_mtod_offset (rx_buff[j], struct rte_ether_hdr *,sizeof(struct rte_ether_hdr) + vlan_tag_hdr );
        udp_hdr = (struct rte_udp_hdr *) rte_pktmbuf_mtod_offset (rx_buff[j], struct rte_ether_hdr *,sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr) + vlan_tag_hdr );
        tot_seg = (uint8_t *)     rte_pktmbuf_mtod_offset (rx_buff[j], uint8_t *,  sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr) + sizeof(struct rte_udp_hdr) + vlan_tag_hdr);
        seg_no  = (uint8_t *)     rte_pktmbuf_mtod_offset (rx_buff[j], uint8_t *,  sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr) + sizeof(struct rte_udp_hdr) + sizeof(uint8_t) + vlan_tag_hdr);
        seg_len = (MsgLen *) rte_pktmbuf_mtod_offset (rx_buff[j], MsgLen *, sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr) + sizeof(struct rte_udp_hdr) + sizeof(uint8_t) + sizeof(uint8_t) + vlan_tag_hdr);
        buf     = (Data *)   rte_pktmbuf_mtod_offset (rx_buff[j], uint8_t *,  sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr) + sizeof(struct rte_udp_hdr) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(MsgLen) + vlan_tag_hdr);
         rte_prefetch0(eth_hdr);
         rte_prefetch0(ipv4_hdr);
         rte_prefetch0(udp_hdr);
         rte_prefetch0(tot_seg);
         rte_prefetch0(seg_no);
         rte_prefetch0(seg_len);
         rte_prefetch0(buf);


         /* Continue - if LLDP pakcets received */
         if ( eth_hdr->ether_type == 0xCC88 )
         {
            rte_pktmbuf_free(rx_buff[j]);
            continue;
         }

         if(gCfg.du_mac_address_validation)
         {
            char duMacAddr[RTE_ETHER_ADDR_FMT_SIZE];
            char duCfgAddr[RTE_ETHER_ADDR_FMT_SIZE];
            rte_ether_format_addr(duMacAddr, RTE_ETHER_ADDR_FMT_SIZE, &eth_hdr->s_addr);
            rte_ether_format_addr(duCfgAddr, RTE_ETHER_ADDR_FMT_SIZE,&(p_pci_eth_dev[0].dst_eth_addr));

            if(strcmp(duMacAddr,duCfgAddr) != 0)
            {
             //  printf(" Packets received are not from DU, discarding the packets from %s\n",duMacAddr);
               printf("\nDiscarded packetd from dst[%s]",duMacAddr);
               rte_pktmbuf_free(rx_buff[j]);
               continue;
            }
         }

         memcpy(bufToSend, buf, *seg_len);
         fapiClProcessUeSim2MacFapiMsg(bufToSend); 
         memset(bufToSend, 0 , MAX_BUF_LEN);

#if 0
         if (*seg_no == 1)
         {
            indx = 1;
            dropPkt = FALSE;
            prmtv = buf[0];
            pst.region = 0;
                 ret = SGetMsg(pst.region, pst.pool, &rcvBuf);
                 if (ret != ROK)
                 {
                   rte_pktmbuf_free (rx_buff[j]);
                   dropPkt = TRUE;
                 }
         }

         if (dropPkt == TRUE)
         {
            continue;
         }

         if (indx != *seg_no)
         {
            printf("Received Out of Order packet, "
                  "expected SN [%d], seg_no [%d] !!!\n Dropping the packet!!!\n", indx, *seg_no);
                 rte_pktmbuf_free (rx_buff[j]);
            dropPkt = TRUE;
            continue;
         }

         indx++;
         recvLen = rte_pktmbuf_pkt_len(rx_buff[j]) - (sizeof(uint8_t) + sizeof(uint8_t)
                    - sizeof(MsgLen) - sizeof(struct rte_ether_hdr));

         if (recvLen < *seg_len)
         {
            printf("Received Packet Length less than the actual size, recvLen [%d], seg_len[%d]\n", recvLen, *seg_len);
                 rte_pktmbuf_free (rx_buff[j]);
            dropPkt = TRUE;
            continue;
         }

         if(indx == 2)
         {
         ret = SAddPstMsgMult(buf, *seg_len, rcvBuf);
         if (ret != ROK)
         {
            SPutMsg(rcvBuf);
            rte_pktmbuf_free (rx_buff[j]);
            dropPkt = TRUE;
            continue;
         }
         }

         if (*tot_seg == *seg_no)
         {
            pst.event = prmtv;

            MsgLen       len, len1;
            len = 0;
            len1 = 0;
            uint8_t *p_rcvd_msg = NULL;
            SFndLenMsg(rcvBuf, &len);
            DU_LOG("\nLength of the Fapi Buffer Received %d, Event:%d\n", len, prmtv);
            len1 = 0;
            ret = SGetSBuf(DFLT_REGION,DFLT_POOL,&p_rcvd_msg, len);
            if (ret != ROK)
            {
               SPutMsg(rcvBuf);
               DU_LOG("\nMemory Allocation Failed for p_rcvd_msg fapi buffer\n");
               RETVALUE(RFAILED);
            }

            if (len != 0)
            {
               SCpyMsgFix(rcvBuf, 0, len, (Data *)p_rcvd_msg, &len1);
            }

            uint16_t msg_id = 0;
            uint16_t index = 0;
            uint16_t opaque_id = 0;
            uint8_t  num_msgs = 0;

            GET_FAPI_L1API_HDR(opaque_id, num_msgs, index, p_rcvd_msg);
            GET_FAPI_MSG_ID(msg_id, index, p_rcvd_msg);

               //Posting to ysActiveTsk
               pst.dstInst = 0;
               pst.prior = PRIOR1;
               SPstTsk(&pst, rcvBuf);
//            SPutSBuf(DFLT_REGION, DFLT_POOL, (Data *)p_rcvd_msg, len);
         }
#endif
         rte_pktmbuf_free (rx_buff[j]);
     }
   }
   RETVALUE(ROK);

}

#ifdef NFAPI_SUPPORT

extern uint16_t gNumOfSlotsPerRadioFrame;
fapiNrTimingInfo_t         *timingInfo = NULLP;
pthread_mutex_t timingInfoLock;

 int32_t dpdk_tti_formation_thread(__attribute__(unused) void *dummy)
{
   U64 cur_tsc = 0;
   U64 prev_tsc = 0;
   U64 diff_tsc = 0;
   fapiNrTimingInfo_t         **gTimingInfo = NULLP;

   Pst pst;
   pst.dstProcId = SFndProcId();
   pst.srcProcId = SFndProcId();
   pst.dstEnt = ENTYS;
   pst.srcEnt = ENTYS;
   pst.srcInst = 0;
   pst.dstInst = 0;
   pst.route = RTESPEC;
   pst.prior = PRIOR0;
   pst.region = DFLT_REGION;
   pst.pool = 3;
   pst.selector = 0;

   uint32_t CPU_Hz = (rte_get_tsc_hz()/US_PER_S);
   U64 TIME_INTERVAL = CPU_Hz * (YS_TTI_TMR_VAL_CFG/MS_PER_S);

   if(SGetSBuf(0, 0, (Data **)&timingInfo, sizeof(fapiNrTimingInfo_t)) != ROK)
   {
      printf("TimingInfo: Failed to allocate memory with size %d...\n", sizeof(fapiNrTimingInfo_t);
      RETVALUE(RFAILED);
   }
   cmMemset(uint8_t*)timingInfo, 0, sizeof(fapiNrTimingInfo_t);

   /* Lets have the timingInfoLock mutex variable to lock the timingInfo
    * with respective places during modification */
   pthread_mutex_init(&timingInfoLock, NULL);

   prev_tsc = rte_rdtsc();

   while (1)
   {
      cur_tsc = rte_rdtsc();
      diff_tsc = cur_tsc - prev_tsc;
      TIME_INTERVAL = CPU_Hz * (YS_TTI_TMR_VAL_CFG/MS_PER_S);

      if (unlikely (diff_tsc >= TIME_INTERVAL))
      {
         if (gTtiReady == TRUE)
         {
            gTimingInfo = &timingInfo;
            uint8_t ret = ROK;
            uint8_t *sendBuf = (uint8_t *)timingInfo;
            Buffer *mBuf = NULLP;
            SGetMsg(DFLT_REGION, DFLT_POOL, &mBuf);
            if(NULLP == mBuf)
            {
               printf("Failed to allocate memory for mBuf\n");
               RETVALUE(RFAILED);
            }
            ret = SAddPstMsgMult((Data *)sendBuf, sizeof(fapiNrTimingInfo_t), mBuf);

            if (ret == ROK)
            {
               pst.prior = PRIOR1;
               pst.dstInst = 0;
               pst.event = EVT_TFU_CL_TTI_INDICATION;
               SPstTsk(&pst, mBuf);
            }

            pthread_mutex_lock(&timingInfoLock);
            if ((*gTimingInfo)->slot + 1) > (gNumOfSlotsPerRadioFrame - 1)
            {
               (*gTimingInfo)->sfn++;
            }
            (*gTimingInfo)->slot = (((*gTimingInfo)->slot + 1) % gNumOfSlotsPerRadioFrame);
            if ((*gTimingInfo)->sfn >= 1024)
            {
               (*gTimingInfo)->hSfn++;
               (*gTimingInfo)->sfn %= 1024;
            }
            pthread_mutex_unlock(&timingInfoLock);
         }
         prev_tsc = cur_tsc;
      }
   }
   pthread_mutex_destroy(&timingInfoLock);
   SPutSBuf(0, 0, (Data *)timingInfo, sizeof(fapiNrTimingInfo_t));

   RETVALUE (ROK);
}
#endif


#ifdef EXT_TGEN_ENABLE
#include "ss_rbuf.h"
#include "ss_rbuf.x"
 void 
eth_send (pci_eth_dev_t *pci_eth_dev)
{
   int16_t ret;
   uint16_t nb_pkts;
   struct rte_ether_hdr *eth_hdr;
   struct rte_ipv4_hdr *ip_hdr;
   uint8_t idx;

   if (pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs)
   {
      nb_pkts = (pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs > MAX_PKT_BURST_TX) ? MAX_PKT_BURST_TX : pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs;

      ret = rte_eth_tx_burst (pci_eth_dev->port_id, 0, pci_eth_dev->tx_mbuf_tbl.tx_mbufs, nb_pkts);

      pci_eth_dev->port_statistics.tx_packets += ret;

      for (idx = 0 ; idx < ret ; idx++)
      {
         eth_hdr = rte_pktmbuf_mtod (pci_eth_dev->tx_mbuf_tbl.tx_mbufs[idx], struct rte_ether_hdr *);
         ip_hdr = (struct rte_ipv4_hdr *)(eth_hdr + 1);

         pci_eth_dev->port_statistics.tx_bytes += rte_bswap16 (ip_hdr->total_length);
      }

      if (unlikely (ret < nb_pkts))
      {
         pci_eth_dev->port_statistics.dropped_packets += (nb_pkts - ret);
         do {
            eth_hdr = rte_pktmbuf_mtod (pci_eth_dev->tx_mbuf_tbl.tx_mbufs[ret], struct rte_ether_hdr *);
            ip_hdr = (struct rte_ipv4_hdr *)(eth_hdr + 1);

            pci_eth_dev->port_statistics.dropped_bytes += rte_bswap16 (ip_hdr->total_length);

            rte_pktmbuf_free (pci_eth_dev->tx_mbuf_tbl.tx_mbufs[ret]);
         } while (++ret < pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs);

      }
      
      pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs -= nb_pkts;
   }
}

 void 
prepare_to_send(pci_eth_dev_t *pci_eth_dev, Buffer **mBuf)
{
   struct rte_mbuf *m;
   MsgLen msgLen;
   MsgLen bufLen;
   int16_t nb_frag_pkts = 0;
   uint8_t i;
   uint8_t ret = ROK;
   S8      *sendBuf;
   struct rte_ether_addr *src_eth_addr = &pci_eth_dev->src_eth_addr;
   //struct rte_ether_addr *dst_eth_addr = &pci_eth_dev->tgen_addr.dst_addr;
   struct rte_ether_addr dst_eth_addr;

   ret = SFndLenMsg(*mBuf, &bufLen);
   if (ret != ROK)
   {
      SPutMsg(*mBuf);
      return;
   }

   m = rte_pktmbuf_alloc (p_rte_direct_mem_pool);

   SPrntMsg(*mBuf, 0, 0);
   sendBuf = rte_pktmbuf_append(m, (bufLen));
   ret = SCpyMsgFix (*mBuf, 0, (bufLen), (Data *)sendBuf, &msgLen);
   uint32_t dstIPAddr = (sendBuf[16] << 24) +  (sendBuf[17] << 16) + (sendBuf[18] << 8) + (sendBuf[19]);			   
   find_arp_entry(htonl(dstIPAddr), &dst_eth_addr);
   //printMacAddres(dst_eth_addr);

   //printf("Pkt Length: %d\n", bufLen);
   SPutMsg(*mBuf);
   if (ret != ROK || msgLen != (bufLen))
   {
      printf("Failed to convert to Flat buffer\n");
      rte_pktmbuf_free(m);
      return;
   }
   if (likely (RTE_ETHER_MTU >= bufLen))
   {
      pci_eth_dev->tx_mbuf_tbl.tx_mbufs[pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs] = m;
      nb_frag_pkts++;
   }
   else
   {
      nb_frag_pkts = rte_ipv4_fragment_packet (m,
                                               &pci_eth_dev->tx_mbuf_tbl.tx_mbufs[pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs],
                                               (uint16_t)(TX_MBUF_TABLE_SIZE - pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs),
                                               RTE_ETHER_MTU,
                                               p_rte_direct_mem_pool, p_rte_indirect_mem_pool);

      rte_pktmbuf_free(m); // Free the input packet as output fragments have been allocated.

      /* If we fail to fragment the packet */
      if (unlikely (nb_frag_pkts < 0))
      {
         printf("IP fragmentation failed\n");
         return;
      }

      printf("Packet is fragmented\n");
   }

   for (i = pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs; 
        i < pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs + nb_frag_pkts; 
        i++)
   {
      struct rte_ether_hdr *eth_hdr = (struct rte_ether_hdr *) 
                                    rte_pktmbuf_prepend (pci_eth_dev->tx_mbuf_tbl.tx_mbufs[i], 
                                    (uint16_t)sizeof(struct rte_ether_hdr));
      if (eth_hdr == NULL)
      {
         rte_panic("No headroom in mbuf.\n");
      }

      pci_eth_dev->tx_mbuf_tbl.tx_mbufs[i]->l2_len = sizeof(struct rte_ether_hdr);

      //rte_memcpy (uint8_t *)eth_hdr, (uint8_t *)&pci_eth_dev->tgen_addr, sizeof(struct rte_ether_hdr);
      rte_ether_addr_copy(&dst_eth_addr, &eth_hdr->dst_addr);
      rte_ether_addr_copy(src_eth_addr, &eth_hdr->src_addr);
      /*
      printf("Src Mac Addr: %02x:%02x:%02x:%02x:%02x:%02x          Dst Mac Addr: %02x:%02x:%02x:%02x:%02x:%02x\n",
            eth_hdr->src_addr.addr_bytes[0], eth_hdr->src_addr.addr_bytes[1], eth_hdr->src_addr.addr_bytes[2], 
            eth_hdr->src_addr.addr_bytes[3], eth_hdr->src_addr.addr_bytes[4], eth_hdr->src_addr.addr_bytes[5],
            eth_hdr->dst_addr.addr_bytes[0], eth_hdr->dst_addr.addr_bytes[1], eth_hdr->dst_addr.addr_bytes[2], 
            eth_hdr->dst_addr.addr_bytes[3], eth_hdr->dst_addr.addr_bytes[4], eth_hdr->dst_addr.addr_bytes[5]);
      */

      eth_hdr->ether_type = rte_be_to_cpu_16(RTE_ETHER_TYPE_IPV4);
   }

   pci_eth_dev->tx_mbuf_tbl.nb_tx_mbufs += nb_frag_pkts;
}

#ifdef ANSI
 S16 extTgenEnDpdkPktRbufInit()
#else
 S16 extTgenEnDpdkPktRbufInit()
#endif
{
   TRC2(extTgenEnDpdkPktRbufInit);

   if(SCreateSRngBuf(SS_RNG_BUF_DLPDCP_TO_DLRLC,0,0,sizeof(SsIpPktDetl),
            SS_RNG_DLPDCP_TO_DLRLC_SIZE) != ROK)
   {
      printf("Ring Buffer Create Failed\n");
      RETVALUE(RFAILED);
   }

   if(SCreateSRngBuf(SS_RNG_PDCP_PKT_TO_DPDK_RBUF, 0, 0, sizeof(PTR),
       SS_RNG_PDCP_PKT_TO_DPDK_RBUF_SIZE) != ROK)
   {
      printf("Ring Buffer Creation Failure\n");
      RETVALUE(RFAILED);
   }

   /* Attach PJ to second ring buffer as Tx entity */
   SAttachSRngBuf(SS_RNG_BUF_DLPDCP_TO_DLRLC, SS_RBUF_ENT_DLPDCP,SS_RNG_TX);
   /* Attach PJ to first ring buffer as Rx entity */
   SAttachSRngBuf(SS_RNG_BUF_DLPDCP_TO_DLRLC, SS_RBUF_ENT_DLRLC,SS_RNG_RX);


   /* Attach PJ to second ring buffer as Tx entity */
   SAttachSRngBuf(SS_RNG_PDCP_PKT_TO_DPDK_RBUF, SS_RBUF_ENT_PDCP_UL_PKT, SS_RNG_TX);
   /* Attach PJ to first ring buffer as Rx entity */
   SAttachSRngBuf(SS_RNG_PDCP_PKT_TO_DPDK_RBUF, SS_RBUF_ENT_DPDK_UL_PKT, SS_RNG_RX);

   RETVALUE(ROK);

}

 int32_t 
dpdk_ring_write(Data **recvBuf, uint32_t recvBufLen)
{
   SsIpPktDetl *tmpBuf;
   void *elem = NULLP;
   void *tcpAckelem = NULLP;
   uint8_t   ipHdrLen = 0;
   uint8_t   tcpFlag = 0;
   uint16_t  protocol = 0;
   Bool isTcpSyncAck =0;

   elem = SRngGetWIndx (SS_RNG_BUF_DLPDCP_TO_DLRLC);
   tcpAckelem = SRngGetWIndx (SS_RNG_BUF_DLPDCP_TO_DLRLC_TCP_ACK);

   ipHdrLen = (*recvBuf[0] & 0x0F) * 4;
   protocol = *recvBuf[9];
   tcpFlag = *recvBuf[ipHdrLen + 13];
   isTcpSyncAck =(tcpFlag & TCP_SYNC_BITMASK) && (tcpFlag & TCP_ACK_FLG_BITMASK);

   if (NULLP != tcpAckelem) && (isTcpSyncAck)
   {
      tmpBuf = (SsIpPktDetl *)tcpAckelem;
      tmpBuf->ul_data = *recvBuf;
      tmpBuf->size = recvBufLen;
      SRngIncrWIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC_TCP_ACK);
      SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC_TCP_ACK].pktRate++;
   }
   else if (NULLP != elem)
   {
      tmpBuf = (SsIpPktDetl *)elem;
      tmpBuf->ul_data = *recvBuf;
      tmpBuf->size = recvBufLen;
      SRngIncrWIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
      SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].pktRate++;
   }
   else
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

 int32_t 
dpdk_ring_read (pci_eth_dev_t *pci_eth_dev)
{
   void *elmIndx = NULLP;
   uint8_t rng_buf_deq_cnt = 0;
   Buffer *mBuf;
   Buffer **tmpBuf;
   if (isRngEmpty(SS_RNG_PDCP_PKT_TO_DPDK_RBUF))
   {
      return RFAILED;
   }   
   /* Read from PDCP Ring */
   elmIndx = SRngGetRIndx (SS_RNG_PDCP_PKT_TO_DPDK_RBUF);
   while (elmIndx != NULLP)
   {
      tmpBuf = (Buffer **)elmIndx;
      mBuf = *tmpBuf;

      prepare_to_send(pci_eth_dev, &mBuf);
      SRngIncrRIndx (SS_RNG_PDCP_PKT_TO_DPDK_RBUF);
      
      rng_buf_deq_cnt++;
      if (rng_buf_deq_cnt >= MAX_PKT_BURST_TX)
      {
         break;
      }
      elmIndx = (void *)SRngGetRIndx (SS_RNG_PDCP_PKT_TO_DPDK_RBUF);
   }
   return ROK;
}

 void
handle_ip_packet(pci_eth_dev_t *pci_eth_dev, struct rte_mbuf *mBuf)
{
   Region region = DFLT_REGION; 
   Pool   pool = 3;                 /* egTLCb Pool */
   struct rte_ipv4_hdr *ip_hdr;
   struct rte_ether_hdr *eth_hdr;
   
   Data     *recvBuf;
   uint32_t recvBufLen;


   eth_hdr = rte_pktmbuf_mtod(mBuf, struct rte_ether_hdr *);
   ip_hdr = (struct rte_ipv4_hdr *)(eth_hdr + 1);

   recvBufLen = rte_bswap16 (ip_hdr->total_length);

   if (SGetSBuf(region, pool, (Data **) &recvBuf, recvBufLen) != ROK)
   {
      printf("EXT_TGEN_ENABLE: Failed to allocate memory for packet of size %d...\n", recvBufLen);
      RETVOID;
   }

   rte_memcpy ((uint8_t *)recvBuf, (uint8_t *)ip_hdr, recvBufLen);

   /* Add to PDCP Ring */
   if (ROK != dpdk_ring_write (&recvBuf, recvBufLen))
   {
      SPutSBuf(region, pool, recvBuf, recvBufLen);
      SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].pktDrop++;
      if (!(SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].pktDrop % 10000) )
      {
         printf("RING-pktDrop=%d\n", SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].pktDrop);
      }

      SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC_TCP_ACK].pktDrop++;
      if (!(SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC_TCP_ACK].pktDrop % 10000) )
      {
         printf("RING-pktDrop=%d\n", SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC_TCP_ACK].pktDrop);
      }
   }
   
   return;
}

 void 
deliver_ip_packet(pci_eth_dev_t *pci_eth_dev, uint8_t j, uint64_t tms)
{
   struct rte_mbuf *mBuf = pci_eth_dev->rx_pkts[j];
   struct rte_ether_hdr *eth_hdr = rte_pktmbuf_mtod(mBuf, struct rte_ether_hdr *);
   struct rte_ip_frag_tbl *frag_tbl = pci_eth_dev->frag_tbl;
   struct rte_ip_frag_death_row *death_row = &pci_eth_dev->death_row;
   struct rte_ipv4_hdr *ip_hdr;
   struct rte_arp_hdr *arp_hdr;
   uint16_t offset = 0;

   /* Process IPv4 packets */
   if (RTE_ETH_IS_IPV4_HDR(mBuf->packet_type))
   {
      ip_hdr = (struct rte_ipv4_hdr *)(eth_hdr + 1);
      //ip_hdr = rte_pktmbuf_mtod_offset(m, struct ipv4_hdr *, sizeof(struct ether_hdr);

      /* If it is a fragmented packet, then try to reassemble. */
      if (rte_ipv4_frag_pkt_is_fragmented(ip_hdr))
      {
         struct rte_mbuf *assembled_mbuf;

         mBuf->l2_len = sizeof(*eth_hdr); // L2 Header Length

         mBuf->l3_len = (ip_hdr->version_ihl & RTE_IPV4_HDR_IHL_MASK) * RTE_IPV4_IHL_MULTIPLIER; // L3 header length
         // mBuf->l3_len = sizeof(*ip_hdr); // L3 header length

         /* Try to assemble the packet with the received fragment*/
         assembled_mbuf = rte_ipv4_frag_reassemble_packet(frag_tbl, death_row, mBuf, tms, ip_hdr);
         if (assembled_mbuf == NULL)
         {
            /* Packet has not been reassembled, so return*/
            return;
         }

         if(assembled_mbuf != mBuf)
         {
            mBuf = assembled_mbuf;
         }
      }

      //eth_hdr = rte_pktmbuf_mtod(mBuf, struct rte_ether_hdr *);
      //ip_hdr = (struct rte_ipv4_hdr *)(eth_hdr + 1);

      pci_eth_dev->port_statistics.rx_bytes += rte_bswap16(ip_hdr->total_length);
      pci_eth_dev->port_statistics.rx_packets++;

      /*
      printf("Src Mac Addr: %02x:%02x:%02x:%02x:%02x:%02x          Dst Mac Addr: %02x:%02x:%02x:%02x:%02x:%02x\n",
              eth_hdr->src_addr.addr_bytes[0], eth_hdr->src_addr.addr_bytes[1], eth_hdr->src_addr.addr_bytes[2], 
              eth_hdr->src_addr.addr_bytes[3], eth_hdr->src_addr.addr_bytes[4], eth_hdr->src_addr.addr_bytes[5],
              eth_hdr->dst_addr.addr_bytes[0], eth_hdr->dst_addr.addr_bytes[1], eth_hdr->dst_addr.addr_bytes[2], 
              eth_hdr->dst_addr.addr_bytes[3], eth_hdr->dst_addr.addr_bytes[4], eth_hdr->dst_addr.addr_bytes[5]);
      */
      //printf("[NG_IO_APP] Received IP packet with SRC-Addr[%x] DST-Addr[%x], delivering to user \n",ip_hdr->src_addr,ip_hdr->dst_addr);

      handle_ip_packet(pci_eth_dev, mBuf);

   }
   else if (eth_hdr->ether_type == rte_cpu_to_be_16(RTE_ETHER_TYPE_ARP))
   {
      printf("Received ARP Packet\n");
      handle_recvd_arp_message(pci_eth_dev, mBuf);
   }
   /* Free input packet */
   rte_pktmbuf_free(mBuf);
}

 void 
eth_recv(pci_eth_dev_t *pci_eth_dev, uint64_t tms)
{
   uint8_t rx_cnt;
   uint8_t j;
   uint16_t portid;

   portid = pci_eth_dev->port_id;
   rx_cnt = rte_eth_rx_burst(portid, 
                            pci_eth_dev->rx_queue_id[0], 
                            pci_eth_dev->rx_pkts, 
                            MAX_PKT_BURST_RX);

#if 0
   if(rx_cnt > 0)
   {
      static int pktcount = 0;
      if (pktcount++ % 100 == 0)
      printf("[NG_IO_APP] Received Packets Count [%u] through port [%u]\n", rx_cnt, pci_eth_dev->port_id);
   }
#endif

   /* Prefetch first packets */
   for (j = 0; j < PREFETCH_OFFSET && j < rx_cnt; j++) 
   {
      rte_prefetch0(rte_pktmbuf_mtod(pci_eth_dev->rx_pkts[j], void *);
   }

   /* Prefetch and forward already prefetched packets */
   for (j = 0; j < (rx_cnt - PREFETCH_OFFSET); j++) 
   {
      rte_prefetch0(rte_pktmbuf_mtod(pci_eth_dev->rx_pkts[j + PREFETCH_OFFSET], void *);

      deliver_ip_packet (pci_eth_dev, j, tms);
   }

   /* Forward remaining prefetched packets */
   for (; j < rx_cnt; j++) 
   {
      deliver_ip_packet(pci_eth_dev, j, tms);
   }

   rte_ip_frag_free_death_row (&pci_eth_dev->death_row, PREFETCH_OFFSET);
}


int32_t ng_main(void *arg)
{
   pci_eth_dev_t *pci_eth_dev = (pci_eth_dev_t *)arg;
   uint64_t cur_tsc;
   uint64_t txdrain_prev_tsc = 0;
   uint64_t txdrain_diff_tsc;
   const uint64_t txdrain_tsc = (rte_get_tsc_hz() + US_PER_S - 1) / US_PER_S * BURST_TX_DRAIN_US;

   uint64_t datarate_prev_tsc = 0;
   uint64_t datarate_diff_tsc;
   const uint64_t datarate_tsc = (rte_get_tsc_hz() + US_PER_S - 1) / US_PER_S * DATARATE_US;

   printf("\n\n[NG_IO_APP] Interface I/O Bootstrapped on lcore[%u] \n",rte_lcore_id());

   for(;;)
   {
      cur_tsc = rte_rdtsc();
      txdrain_diff_tsc = cur_tsc - txdrain_prev_tsc;
      datarate_diff_tsc = cur_tsc - datarate_prev_tsc;

      //Receive
      eth_recv(pci_eth_dev, cur_tsc);
      if (ROK == dpdk_ring_read (pci_eth_dev))
      {
         eth_send (pci_eth_dev);
      }

      if (unlikely(datarate_diff_tsc > datarate_tsc))
      {
         print_stats(pci_eth_dev);
         datarate_prev_tsc = cur_tsc;
      }
   }
   return 0;
}

 void hash_init(void)
{
   struct rte_hash_parameters hash_params =  { 0 }; 
   {
      hash_params.name = "IP2MAC_TABLE";
      hash_params.entries = MAX_CELLS * UE_NUM_PER_CELL;
      hash_params.key_len = sizeof(uint32_t);
      hash_params.hash_func_init_val = 0;
      hash_params.socket_id = rte_socket_id();
   };
   
   ip2mac_table = rte_hash_create(&hash_params);
   if (NULL == ip2mac_table)
   {
      rte_panic("Failed to create IP2MAC_TABLE hash table, errno = %d\n", rte_errno);
      printf("Failed to create IP2MAC_TABLE hash table\n");
      //exit(1);
   }
   else
   {
      printf("IP2MAC_TABLE hash MAP created\n");  
   }
}

/* *
 * *
 * *       Fun:   send_arp_req
 * *       
 * *       Desc:  DPDK based ARP utility for sending ARP request
 * *       
 * *       Ret:   ROK
 * *       
 * *       Notes: None
 * *              
 * *       File:  ss_dpdk.c 
 * *       
 * */

 bool send_arp_req(uint16_t eth_port_id, uint32_t dst_ip_addr, uint32_t src_ip_addr)
{
   struct rte_ether_hdr *eth_hdr;
   struct rte_arp_hdr *arp;
   struct rte_mbuf *mbuf;
   struct rte_ether_addr eth_src_addr;
   printf("In function send_arp_req\n");
   rte_eth_macaddr_get(eth_port_id, &eth_src_addr);

   mbuf = rte_pktmbuf_alloc(p_rte_direct_mem_pool);
   if (mbuf == NULL)
       return false;

   eth_hdr = (struct rte_ether_hdr *)rte_pktmbuf_append(mbuf, ARP_PKT_SIZE - 4);
   if (eth_hdr == NULL) 
   {
       rte_pktmbuf_free(mbuf);
       return false;
   }

   /* Ethernet header. */
   rte_ether_addr_copy(&eth_src_addr, &eth_hdr->src_addr);
   ngp::checked_memset(eth_hdr->dst_addr.addr_bytes, 0xff, RTE_ETHER_ADDR_LEN);
   eth_hdr->ether_type = rte_be_to_cpu_16(RTE_ETHER_TYPE_ARP);

   /* ARP header. */
   arp = (struct rte_arp_hdr *)(eth_hdr + 1);
   arp->arp_hardware = htons(RTE_ARP_HRD_ETHER);
   arp->arp_protocol = htons(RTE_ETHER_TYPE_IPV4);
   arp->arp_hlen = RTE_ETHER_ADDR_LEN;
   arp->arp_plen = 4;
   arp->arp_opcode  = htons(RTE_ARP_OP_REQUEST);
 
   rte_ether_addr_copy(&eth_src_addr, &arp->arp_data.arp_sha);
   ngp::checked_memset(arp->arp_data.arp_tha.addr_bytes, 0x00, RTE_ETHER_ADDR_LEN);
   rte_memcpy(&arp->arp_data.arp_sip, &src_ip_addr, 4);
   rte_memcpy(&arp->arp_data.arp_tip, &dst_ip_addr, 4);

   printf("Sending ARP request for IP: %x\n", dst_ip_addr);
   rte_eth_tx_burst(eth_port_id, 0, &mbuf, 1);
   
   return true;
}


 void handle_recvd_arp_message(pci_eth_dev_t *pci_eth_dev,struct rte_mbuf *mBuf)
{
   struct rte_ether_hdr *eth_hdr = rte_pktmbuf_mtod(mBuf, struct rte_ether_hdr *);
   struct rte_arp_hdr *arp_hdr = (struct rte_arp_hdr *)(char *)(eth_hdr + 1);
   printf("Received ARP packet with IP SourceIP %x TargetIP %x \n", arp_hdr->arp_data.arp_sip, arp_hdr->arp_data.arp_tip);
   printf("Uesim Selfif: %x\n", gCfg.pci_conf.uesim_to_vc_intf_ip);
   //Target ip Matches with IP configured locally
   if (arp_hdr->arp_data.arp_tip == gCfg.pci_conf.uesim_to_vc_intf_ip)
   {
      printf("ARP targetIP %x selfIp %x matches\n", arp_hdr->arp_data.arp_tip, gCfg.pci_conf.uesim_to_vc_intf_ip);
      if (arp_hdr->arp_opcode == rte_cpu_to_be_16(RTE_ARP_OP_REQUEST))
      {
        //Send ARP response
        arp_hdr->arp_opcode = rte_cpu_to_be_16(RTE_ARP_OP_REPLY);

        rte_ether_addr_copy(&eth_hdr->src_addr, &eth_hdr->dst_addr);
        rte_ether_addr_copy(&p_pci_eth_dev[NG_PCI_ADDR].src_eth_addr, &eth_hdr->src_addr);

        rte_ether_addr_copy(&arp_hdr->arp_data.arp_sha, &arp_hdr->arp_data.arp_tha);
        arp_hdr->arp_data.arp_tip = arp_hdr->arp_data.arp_sip;

        rte_ether_addr_copy(&p_pci_eth_dev[NG_PCI_ADDR].src_eth_addr, &arp_hdr->arp_data.arp_sha);
        arp_hdr->arp_data.arp_sip = gCfg.pci_conf.uesim_to_vc_intf_ip;

        printf("Sending ARP Response SrcIP: %x DstIP: %x ", arp_hdr->arp_data.arp_sip, arp_hdr->arp_data.arp_tip);
        printf("SRC MAC: ");
        printMacAddres2(eth_hdr->src_addr);
        printf("DST MAC: ");
        printMacAddres2(eth_hdr->dst_addr);
      
        //Sending ARP response
        rte_eth_tx_burst(pci_eth_dev->port_id, 0, &mBuf, 1);
      }
      else if (arp_hdr->arp_opcode == rte_cpu_to_be_16(RTE_ARP_OP_REPLY)) 
      {
         //Store the IP and MAC address in ip2mac_table hash table
         uint32_t ip_addr = arp_hdr->arp_data.arp_sip;
         printf("ARP Response Received Store in hash table [IP:%x]", ip_addr);
         //check for memory leak, and see who is responsible for freeing memory, instead can we use local storage
         struct rte_ether_addr *eth_addr = malloc(sizeof(struct rte_ether_addr);
         //struct rte_ether_addr eth_addr;
         rte_ether_addr_copy(&arp_hdr->arp_data.arp_sha, eth_addr);
         printMacAddres(eth_addr); 
         rte_hash_add_key_data(ip2mac_table, &ip_addr, (void *)eth_addr);
         eth_addr = NULL;
         rte_hash_lookup_data(ip2mac_table, &ip_addr, (void **)&eth_addr);
         printMacAddres(eth_addr); 
      }
   }
}
/* *
 * *
 * *       Fun:   find_arp_entry
 * *       
 * *       Desc:  DPDK based ARP utility for finding mac address based on ip address and pci address
 * *       
 * *       Ret:   ROK
 * *       
 * *       Notes: None
 * *              
 * *       File:  ss_dpdk.c 
 * *       
 * */

 bool find_arp_entry(uint32_t ip_addr, struct rte_ether_addr *eth_addr2)
{
   //struct rte_ether_addr *eth_addr = malloc(sizeof(struct rte_ether_addr);
   struct rte_ether_addr *eth_addr = NULL;
   //struct rte_ether_addr eth_addr;

   int ret = rte_hash_lookup_data(ip2mac_table, &ip_addr, (void **)&eth_addr);	
   if (ret < 0)
   {
      printf("Arp Entry Not Found, ret: %d", ret);
      return false;
   }
   printf("ARP entry Found for IP: %x", ip_addr);
   printMacAddres(eth_addr);
   rte_ether_addr_copy(eth_addr, eth_addr2);
   return true;
}

 void dpdk_arp_request(uint32_t dst_ip_addr)
{
  printf("dpdk_arp_request: Sending ARP request: %x\n", dst_ip_addr);
  send_arp_req(p_pci_eth_dev[NG_PCI_ADDR].port_id, htonl(dst_ip_addr), gCfg.pci_conf.uesim_to_vc_intf_ip); 
}

#endif

 int lsi_event_callback(uint16_t port_id, enum rte_eth_event_type msg_type, 
                                void *param, void *ret_param)
{
   struct rte_eth_link lk_status;
   
   RTE_SET_USED(param);
   RTE_SET_USED(ret_param);

   printf(" Link Status Update ...\n");
   printf(" Event type : %s\n", msg_type == RTE_ETH_EVENT_INTR_LSC?"LSC IRQ":"unknown event");
   rte_eth_link_get(port_id, &lk_status);
   if (lk_status.link_status) {
      printf("Port %d Link Up - speed %u Mbps - %s\n\n",\
            port_id, (unsigned)lk_status.link_speed,\
            (lk_status.link_duplex == ETH_LINK_FULL_DUPLEX) ?\
            ("full-duplex") : ("half-duplex"));
   } else
      printf("Port %d Link Down\n\n", port_id);

   return 0;
}

/* Builds up the correct configuration for vmdq+dcb based on the vlan tags array
 * given above, and the number of traffic classes available for use. */
int16_t get_eth_conf(struct rte_eth_conf *eth_conf, enum rte_eth_nb_pools npools)
{
   struct rte_eth_vmdq_dcb_conf conf;
   uint16_t i;

   if (npools != ETH_16_POOLS && npools != ETH_32_POOLS ) return -1;

   conf.nb_queue_pools = npools;
   conf.enable_default_pool = 0;
   conf.default_pool = 0; /* set explicit value, even if not used */
   conf.nb_pool_maps = sizeof( vlan_tags )/sizeof( vlan_tags[ 0 ]);
   for (i = 0; i < conf.nb_pool_maps; i++)
   {
      conf.pool_map[i].vlan_id = vlan_tags[ i ];
      conf.pool_map[i].pools = 1 << (i % npools);
   }
   for (i = 0; i < ETH_DCB_NUM_USER_PRIORITIES; i++)
   {
      conf.dcb_tc[i] = (uint8_t)(i % (NUM_RX_QUEUES/npools));
   }
   (void)(rte_memcpy(eth_conf, &portcfg, sizeof(*eth_conf)));
   (void)(rte_memcpy(&eth_conf->rx_adv_conf.vmdq_dcb_conf, &conf,
                           sizeof(eth_conf->rx_adv_conf.vmdq_dcb_conf)));
   return 0;
}

 int16_t clone_pci_eth_dev (pci_eth_dev_t *p_eth_dev, const S8 *pci_addr_str)
{
   uint8_t is_valid_pci = FALSE;
   uint16_t nb_ports;
   struct rte_pci_addr pci_addr;
   struct rte_eth_dev_info dev_info;

   if (0 != rte_pci_addr_parse(pci_addr_str, &pci_addr))
   {
      printf("rte_pci_addr_parse failed for: %s\n", pci_addr_str);
      return -1;
   }

   nb_ports = rte_eth_dev_count_avail();
   if (0 == rte_eth_dev_get_port_by_name(pci_addr_str, &p_eth_dev->port_id))
   {
	   printf("\n[NG_IO_APP] Found port [%u], corresponding to cloned PCI-ETH-DEV\n\n", p_eth_dev->port_id);
	   printf("Initializing port %u... ", p_eth_dev->port_id);
	   is_valid_pci = TRUE;
   }

   if (!is_valid_pci)
   {
      printf("Error: Invalid PCI! No compatible DPDK ports\n");
      return -1;
   }

   rte_memcpy (p_eth_dev->pci_address, pci_addr_str, MAX_PCI_ADDRESS_SIZE);
   return 0;
}

 int32_t dpdk_eal_init()
{
   int32_t ret;
   uint8_t i;
   uint8_t nb_ports;
   uint16_t portid;
   const S8 *pci_addr_str;
   const S8 *pci_dst_mac_addr;
   const S8 *tgen_src_mac_addr;
   const S8 *tgen_dst_mac_addr;
   uint8_t len = 0; 

   uint16_t u_core_mask = 0x3e;
   S8 *argv[8];
   int16_t argc;

   argv[0] = (S8 *)malloc(32);
   argv[1] = (S8 *)malloc(32);
   argv[2] = (S8 *)malloc(32);
   argv[3] = (S8 *)malloc(32);
   argv[4] = (S8 *)malloc(32);
   argv[5] = (S8 *)malloc(32);
   argv[6] = (S8 *)malloc(32);
   argv[7] = (S8 *)malloc(32);

   sprintf(argv[0], "gnodeb");
   //sprintf(argv[1], "-c %x", u_core_mask);
   sprintf(argv[1], "-m 1024");
   snprintf(argv[2], 32, "-n %d", gCfg.thread_conf.n);
   snprintf(argv[3], 32, "--file-prefix=%s", gCfg.hugepage_conf_file);
   len = snprintf(argv[4], 32, "-l %d", gCfg.thread_conf.eal_tid);

   argc = 5;
   snprintf(argv[5], 32, "--vdev=%s", gCfg.pci_address); // --vdev will create virtual device of name pci_address
   argc++;
   if (gCfg.is_fronthaul_en)
   {
      len += snprintf(argv[4] + len, 32 - len, ",%d", gCfg.thread_conf.rx_tid);
#ifdef NFAPI_SUPPORT
      len += snprintf(argv[4] + len, 32 - len, ",%d", gCfg.thread_conf.tti_tmr_tid);
#endif
   }
   
   if (gCfg.is_backhaul_en)
   {
      len += snprintf(argv[4] + len, 32 - len, ",%d",
                      gCfg.thread_conf.tgen_rx_tid);
   }
#if 0
   //Deprecating the blacklist concept for now
  if (gCfg.is_blacklist_en)
   {
      snprintf(argv[6], 32, "-b %s", gCfg.pci_addr_to_blacklist);
      argc++;
      printf("Blacklisted PCI dev: %s\n", gCfg.pci_addr_to_blacklist);
   }
#endif

   //Add all the allowed PCI list inside the whitelist of EAL.Currently enabling only the fronthaul port
   if(gCfg.is_fronthaul_en)
   {
      snprintf(argv[6],32, "-w%s", gCfg.pci_conf.pci_addr_fronthaul);
      argc++;
      printf("Allowed PCI dev for franthaul: %s\n",gCfg.pci_conf.pci_addr_fronthaul);
   }
   if(gCfg.is_backhaul_en)
   {
      snprintf(argv[7],32, "-w%s", gCfg.pci_conf.pci_addr_backhaul);
      argc++;
      printf("Allowed PCI devi for backhaul: %s\n",gCfg.pci_conf.pci_addr_backhaul);
   }

   struct rte_eth_conf port_conf;

   /* init EAL */
   ret = rte_eal_init(argc, argv);
   if (ret < 0)
   {
      printf("rte_eal_init failed\n");
      rte_exit(EXIT_FAILURE, "rte_eal_init failed\n");
   }

   if(gCfg.is_fronthaul_en)
   {
      /* create the mbuf pool */
      bbuio_buf_pool = rte_mempool_create("l2fwd_mem", 65536, MBUFSZ, 256, \
            sizeof(struct rte_pktmbuf_pool_private), \
            rte_pktmbuf_pool_init, NULL,
            rte_pktmbuf_init, NULL, 0, 0);

      if (bbuio_buf_pool == NULL)
         rte_exit(EXIT_FAILURE, "Cannot init mbuf pool\n");

      dpdk_mem_pools[RX_L2_FWD_MEM_POOL] = bbuio_buf_pool;
   }

   if (gCfg.is_backhaul_en)
   {
      p_rte_direct_mem_pool = rte_pktmbuf_pool_create (DIRECT_MEMPOOL_NAME, 65536, 32, \
                              0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());

      p_rte_indirect_mem_pool = rte_pktmbuf_pool_create (INDIRECT_MEMPOOL_NAME, 65536, 32, \
                              0, 0, rte_socket_id());

      if (p_rte_direct_mem_pool == NULL || p_rte_indirect_mem_pool == NULL)
         rte_exit(EXIT_FAILURE, "Cannot init mbuf pool\n");

      dpdk_mem_pools[RX_NG_MEM_POOL] = p_rte_direct_mem_pool;
   }

   ret = get_eth_conf(&port_conf, num_pools);
   if(ret != 0)
      return ret;

   nb_ports = rte_eth_dev_count_avail();
   if (nb_ports == 0)
      rte_exit(EXIT_FAILURE, "No Ethernet ports - bye\n");

   printf("Number of ports: %d\n", nb_ports);

   for (i = 0 ; i < nb_ports ; i++)
   {
      if (i == L2_FWD_PCI_ADDR)
      {
         pci_addr_str = gCfg.pci_conf.pci_addr_fronthaul;
         pci_dst_mac_addr = gCfg.pci_conf.dst_mac_addr_fh;
         if (!gCfg.is_fronthaul_en)
         {
            i++;
            pci_addr_str = gCfg.pci_conf.pci_addr_backhaul;
            tgen_src_mac_addr = gCfg.pci_conf.tgen_src_mac_addr;
            tgen_dst_mac_addr = gCfg.pci_conf.tgen_dst_mac_addr;
            rte_memcpy (p_pci_eth_dev[i].tgen_addr.s_addr.addr_bytes, 
                        tgen_src_mac_addr, 
                        sizeof(struct rte_ether_addr));
            rte_memcpy (p_pci_eth_dev[i].tgen_addr.d_addr.addr_bytes, 
                        tgen_dst_mac_addr, 
                        sizeof(struct rte_ether_addr));
            p_pci_eth_dev[i].tgen_addr.ether_type = htons(ETHERNET_TYPE);
         }
      }
#ifdef EXT_TGEN_ENABLE
      else if (gCfg.is_backhaul_en && (i == NG_PCI_ADDR))
      {
         pci_addr_str = gCfg.pci_conf.pci_addr_backhaul; 
              p_pci_eth_dev[i].pci_address[0],
              p_pci_eth_dev[i].pci_address[0],
         tgen_src_mac_addr = gCfg.pci_conf.tgen_src_mac_addr;
         tgen_dst_mac_addr = gCfg.pci_conf.tgen_dst_mac_addr;
         pci_dst_mac_addr  = gCfg.pci_conf.tgen_dst_mac_addr;
         rte_memcpy (uint8_t *)p_pci_eth_dev[i].tgen_addr.src_addr.addr_bytes, 
                     (uint8_t *)tgen_src_mac_addr, 
                     sizeof(struct rte_ether_addr);
         rte_memcpy (uint8_t *)p_pci_eth_dev[i].tgen_addr.dst_addr.addr_bytes, 
                     (uint8_t *)tgen_dst_mac_addr, 
                     sizeof(struct rte_ether_addr);
         p_pci_eth_dev[i].tgen_addr.ether_type = htons(ETHERNET_TYPE);
      }
#endif
      else
      {
         continue;
      }
      clone_pci_eth_dev(&p_pci_eth_dev[i], pci_addr_str); 
      printf(" PORT ID: %d PCI: %s\n",p_pci_eth_dev[i].port_id, pci_addr_str);

      portid = p_pci_eth_dev[i].port_id;
      p_pci_eth_dev[i].nb_rx_queues = 1;
      p_pci_eth_dev[i].rx_queue_id[0] = 0;
      p_pci_eth_dev[i].nb_tx_queues = 1;
      p_pci_eth_dev[i].tx_queue_id[0] = 0;

      p_pci_eth_dev[i].nb_rx_ring_dsc = 128;
      p_pci_eth_dev[i].nb_tx_ring_dsc = 512;

      p_pci_eth_dev[i].port_config = port_conf;
      p_pci_eth_dev[i].tx_mbuf_tbl.nb_tx_mbufs = 0;

      ret = rte_eth_dev_configure (portid, 
                                   p_pci_eth_dev[i].nb_rx_queues, 
                                   p_pci_eth_dev[i].nb_tx_queues, 
                                   &port_conf);

      if (ret < 0)
         rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d, port=%u\n",
                       ret, portid);

      rte_eth_dev_callback_register (portid, RTE_ETH_EVENT_INTR_LSC, lsi_event_callback, NULL);

      /* NEED FIX HERE, ADDED TEMPORARY FIX FOR TREX */
      ret = rte_eth_rx_queue_setup(portid, 0, nb_rxd,
                                   rte_eth_dev_socket_id(portid),
                                   &rx_cfg,
                                   (struct rte_mempool *)dpdk_mem_pools[i]);
 
      if (ret < 0)
         rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n",
                            ret, (uint16_t)portid);

      ret = rte_eth_tx_queue_setup(portid, 0, nb_txd,
                      rte_eth_dev_socket_id(portid),
                      &tx_cfg);
      if (ret < 0)
         rte_exit(EXIT_FAILURE, "rte_eth_tx_queue_setup:err=%d, port=%u\n",
                      ret, (uint16_t)portid);

      /* Start device */
      ret = rte_eth_dev_start(portid);
      if (ret < 0)
         rte_exit(EXIT_FAILURE, "rte_eth_dev_start:err=%d, port=%u\n",
                        ret, (uint16_t)portid);

      printf("done: \n");

      rte_eth_promiscuous_enable(portid);

      rte_eth_macaddr_get(portid, &p_pci_eth_dev[i].src_eth_addr);
      rte_memcpy(p_pci_eth_dev[i].dst_eth_addr.addr_bytes, 
                 pci_dst_mac_addr, MAX_MAC_ADDRESS_SIZE);

      printf("Port: %u, MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n\n",
                      (uint16_t) portid,
                      p_pci_eth_dev[i].src_eth_addr.addr_bytes[0],
                      p_pci_eth_dev[i].src_eth_addr.addr_bytes[1],
                      p_pci_eth_dev[i].src_eth_addr.addr_bytes[2],
                      p_pci_eth_dev[i].src_eth_addr.addr_bytes[3],
                      p_pci_eth_dev[i].src_eth_addr.addr_bytes[4],
                      p_pci_eth_dev[i].src_eth_addr.addr_bytes[5]);

      memset(&p_pci_eth_dev[i].port_statistics, 0, sizeof(port_statistics_t));
   }

   /* initialize port stats */

   RETVALUE(ROK);
}


int32_t dpdk_init(void)
{
   int32_t ret = ROK;

      printf("PBORLA\n");
#ifdef EXT_TGEN_ENABLE
   extTgenEnDpdkPktRbufInit();
#endif
   printf("==============================================\n");
   printf("        DPDK MODE COMM: UESIM <==> DU         \n");
   printf("==============================================\n");
   ret = dpdk_config_init(&gCfg);
   if (ret != ROK)
   {
      printf("DPDK Config Init Failed\n");
      RETVALUE(RFAILED);
   }

      printf("PBORLA\n");
   ret = dpdk_eal_init();
   if (ret != ROK)
   {
      printf("EAL Initialization Failure\n");
      RETVALUE(RFAILED);
   }
   else
      printf("PBORLA dpdk_eal_init\n");
/* Filling fast_crypto_init params */
#ifdef FAST_CRYPTO_ENABLED
if(gCfg.enable_crypto_init)
{
   ngp::fast_crypto_init_config fast_crypto_init_cfg;
   ngp::checked_memset(&fast_crypto_init_cfg, 0, sizeof(ngp::fast_crypto_init_config));

   fast_crypto_init_cfg.num_devs = 1;/*number of ports*/
   std::memcpy(static_cast<void*>(fast_crypto_init_cfg.dev_cfg[0].device_name),gCfg.pci_address,sizeof(uint8_t)*32);
   std::memcpy(pci_add ,gCfg.pci_address,sizeof(uint8_t)*32);
   fast_crypto_init_cfg.dev_cfg[0].fast_crypto_dev_type = ngp::fast_crypto_dev_type::SOFTWARE;
   fast_crypto_init_cfg.dev_cfg[0].num_sessions = gCfg.max_num_crypto_session;
   fast_crypto_init_cfg.dev_cfg[0].num_queues = 3;
   fast_crypto_init_cfg.fast_crypto_operation_type = ngp::fast_crypto_operation_type::SYMMETRIC;
   ngp::fast_crypto::get_instance()->init(fast_crypto_init_cfg);
   ngp::data_pool::get_instance()->create_pool("ngp_mem_config.txt");
   if(gCfg.crypto_thread_enable)
   {
      FAST_CRYPTO_THREAD_ENABLE = true;
   }
   FAST_CRYPTO_AS_SECURITY = true;
}
#endif
/* Filling fast_crypto_init params end*/

      printf("PBORLA\n");
   if (gCfg.is_fronthaul_en)
   {
      rte_eal_remote_launch(dpdk_rx_thread, NULL, gCfg.thread_conf.rx_tid);

#ifdef NFAPI_SUPPORT
      rte_eal_remote_launch(dpdk_tti_formation_thread, NULL, gCfg.thread_conf.tti_tmr_tid);
#endif
   }

#ifdef EXT_TGEN_ENABLE
   if (gCfg.is_backhaul_en)
   {
      int      sock;
      uint32_t frag_cycles;
      uint32_t max_flow_num = 64;
      uint32_t max_flow_ttl = 10;
      
      hash_init();
      sock = rte_eth_dev_socket_id(p_pci_eth_dev[1].port_id);
      if (sock == SOCKET_ID_ANY)
      {
         sock = 0;
      }

      frag_cycles = (rte_get_tsc_hz() + MS_PER_S - 1) / MS_PER_S * max_flow_ttl;
      
      if (p_pci_eth_dev[NG_PCI_ADDR].frag_tbl = rte_ip_frag_table_create (max_flow_num, 
                                                             IP_FRAG_TBL_BUCKET_ENTRIES, 
                                                             max_flow_num, 
                                                             frag_cycles, sock) == NULL)
      {
         printf("Failed to create the Fragmentation Table\n");
      }
      rte_eal_remote_launch (ng_main, &p_pci_eth_dev[NG_PCI_ADDR], gCfg.thread_conf.tgen_rx_tid);
   }
#endif

      printf("PBORLA\n");
   RETVALUE(ret);
}

#endif
