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
 
     Name:     System Services -- Message manipulation functions
 
     Type:     C source file
 
     Desc:     Macro definitions for message related functions.
 
     File:     ss_dpdk.h
 
     Sid:      ss_dpdk.h@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:44:12 2014
 
     Prg:      sn
 
*********************************************************************21*/
#ifdef DPDK_ENABLE
#ifndef __SSDPDKH__
#define __SSDPDKH__
#include <rte_config.h>
#include <rte_common.h>
#include <rte_log.h>
#include <rte_memory.h>
#include <rte_memcpy.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_launch.h>
#include <rte_atomic.h>
#include <rte_cycles.h>
#include <rte_prefetch.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>
#include <rte_debug.h>
#include <rte_bus_pci.h>
#include <rte_ether.h>
#include <rte_ethdev.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_ip_frag.h>
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PKT_BURST_TX 128
#define MAX_PKT_BURST_RX 128
#define MBUFSZ   (9216 + sizeof(struct rte_mbuf) + RTE_PKTMBUF_HEADROOM)
#define NUM_RX_QUEUES 128
/* How many objects (mbufs) to keep in per-lcore mempool cache */
#define MEMPOOL_CACHE_SZ        32
#define ETHERNET_TYPE (0x0800)
#define RX_THREAD_AFFIINITY 1

/*
 * Configurable number of RX/TX ring descriptors
 */
#define RTE_TEST_RX_DESC_DEFAULT 128
#define RTE_TEST_TX_DESC_DEFAULT 512

/* NIC interface config parameters */

#define RX_PTHRESH 8
#define RX_HTHRESH 8
#define RX_WTHRESH 0


#define TX_PTHRESH 32
#define TX_HTHRESH 0
#define TX_WTHRESH 0

#define PORT_ID                    0

#define PREFETCH_OFFSET  3
#define BURST_TX_DRAIN_US 100 /* TX drain every ~100us */
#define DATARATE_US 10000000 /* Datarate  every 10s */
#define MAX_FRAG_PER_PACKET RTE_LIBRTE_IP_FRAG_MAX_FRAG
#define TX_MBUF_TABLE_SIZE  (2 * (MAX_PKT_BURST_TX > MAX_FRAG_PER_PACKET? MAX_PKT_BURST_TX : MAX_FRAG_PER_PACKET))

#define DPDK_MAX_CFG_PARAM_LEN     256
#define DPDK_MAX_CFG_FILE_LINE_LEN 256

#define MAX_PCI_ADDRESS_SIZE       16
#define MAX_MAC_ADDRESS_SIZE       6
#define MAX_RX_QUEUES_PER_PORT     1
#define MAX_TX_QUEUES_PER_PORT     1
#define MAX_IP_ADDRESS_SIZE        16

#define IP_FRAG_TBL_BUCKET_ENTRIES 2
#define DIRECT_MEMPOOL_NAME        "GNB_MBUF_DIRECT_POOL"
#define INDIRECT_MEMPOOL_NAME      "GNB_MBUF_INDIRECT_POOL"
#define DPDK_SEG_HDR_LEN            8

#ifdef __cplusplus
}
#endif

#endif
#endif
