/*******************************************************************************
################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

/********************************************************************20**

     Name:     Radisys Logging Framework
     Type:     C include file
     Desc:     This file contains logging framework include file for library.
     File:     rl.h

*********************************************************************21*/
/****************************************************************************
@ description: This is header file is used by logging framework module. This
file should not be included by any other application. This is internal
header file logging framework.
*****************************************************************************/

#ifndef __RL_H__
#define __RL_H__

#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "rl_common.h"

#define MAX_FILE_SIZE  			3145728 /* 3MB */
#define MAX_LOG_LEN				256
#define MAX_FILENAME_LEN 		300
#define LOG_TIME_LEN 			64
#define MAX_LOG_BUF_SIZE 		5000
#define RLOG_MAX_CIRBUF_SIZE			(1024*100)
#define RLOG_REMOTE_LOGGING_PORT		9099
#define RLOG_MAX_FILES		 			5
#define RLOG_MAX_TIME_STAMP 			80
#define RLOG_MAX_TAX_NAME				16
#define RLOG_FIFO_FILE					"/tmp/l2logs"
#define RLOG_CIRBUF_READ_INTERVAL	1	/* 60 seconds read interval */
#define RLOG_MAX_THREADS				16
#define RLOG_TIME_ZONE_LEN				8
#define RLOG_MAX_STACK_DEPTH 		24
#define RLOG_MAX_BACKTRACE_BUFSZ	2048
#define RLOG_READ_POS_THRESHOLD 300
#define RLOG_FIXED_LENGTH_BUFFER_SIZE 50
#define RLOGTICKSCNTTOPRCL2LOGS 10

/* Console handling */
#define RLOG_CTRL_L    12
#define RLOG_CTRL_Y    25
#define RLOG_ENTER_KEY 10
#define RLOG_SET_LOGLEVEL  1
#define RLOG_SET_MODMASK   2

/*L2 Logging */
#define PROCESS_L2LOG_TTI 10

typedef enum {
LOG_ARG_INT,
LOG_ARG_STR,
LOG_ARG_HEX,
LOG_ARG_SPL
} LOG_ARG_TYPE;

typedef enum rlLogCntLmt
{
   RL_LOG_COUNT_LIMIT_START = 1,
   RL_LOG_COUNT_LIMIT_STOP
}RLLogCntLmt;

#ifndef RLOG_ENABLE_TEXT_LOGGING

typedef enum
{
	L_TIME_REFERENCE=0,
	L_TIME_DELIMITER,
	L_SIGSEGV,
    L_TIME_TTI_UPDT
} LOGID_TYPE;

typedef struct
{
	time_t tv_sec;
	uint32_t	ms_tti;
} __attribute__ ((__packed__)) LOGTIME;

typedef struct
{
	LOGID			logId;		/* 4 bytes | 32 bits */
	LOGTIME		logTime;		/* 8 bytes */
	unsigned int argType 	:2;
	unsigned int logLevel	:3;
	unsigned int numOfArgs	:3;
	uint16_t			len;
} __attribute__ ((__packed__)) LOGDATA;

typedef struct
{
	LOGDATA 	logData;
	char 		buf[MAX_LOG_BUF_SIZE];
}  __attribute__ ((__packed__)) ARGDATA;

typedef struct
{
	LOGDATA logData;
	uint32_t arg1;
	uint32_t arg2;
	uint32_t arg3;
	uint32_t arg4;
   char unusedByte[19]; /* To make it as 50 byte */
}  __attribute__ ((__packed__)) ARG4DATA;

typedef struct
{
	LOGDATA logData;
	uint8_t		  splEnum;
	uint32_t splArg;
	uint32_t arg1;
	uint32_t arg2;
	uint32_t arg3;
	uint32_t arg4;
   char unusedByte[14]; /* To make it as 50 byte */
}  __attribute__ ((__packed__)) SPL_ARGDATA;

typedef enum _endian {little_endian, big_endian} EndianType;

typedef struct
{
	uint16_t			version;
	uint32_t			dummy32;
	uint8_t				endianType;
	char			szTimeZone[RLOG_TIME_ZONE_LEN+1];
	uint16_t			END_MARKER;
   time_t      time_sec;
} __attribute__ ((__packed__)) FILE_HEADER;

#endif /* RLOG_ENABLE_TEXT_LOGGING */

typedef struct {
	
	char	szTaskName[RLOG_MAX_TAX_NAME];
	uint8_t*	logBuff;	 	/* LOG Buffer */
	uint32_t 	logBufLen;  /* Data Written till now */
	uint32_t	logReadPos; /* Reader thread position */
	uint8_t 	listIndex;	/* Index to global list */

} THREAD_DATA;

void readL2LogBuff(void);
void processL2LogBuff(void);
S16  sendL2LogBuftoL3(void);
void rlInitL2Log(void);
/* Extern for soc specific file */
void rlProcessLogBufFromL2(void *mBuf);
void rlInitL2SocSpecific(void);
//void processL2LogBuff(void);
void rlProcessTicks(void);
void rlGetL2LogBufPtr (void *mBuf, uint32_t *logLen,uint8_t **logPtr);
void rlInvalidateL2LogsInCache(uint8_t *ptr,uint32_t len);

uint8_t     *g_l2rlogBuf;        /* buffer pointer for shared memory allocation */
uint8_t     *g_l2LogBufStartPtr; /* buffer pointer where logs has to be written */
uint8_t     *g_l2LogBufBasePtr;  /* Base pointer for log buffer */
uint8_t     *g_logBufRcvdFromL2; /* Buffer pointer received from L2 at L3*/
uint8_t     *g_l2LogBaseBuff;    /* Base log buffer received at L3 */
uint32_t     g_l2LogBufLen;      /* Log Buffer length written at L2 */
uint32_t     startL2Logging;     /* flag to start processing of L2 logs */
uint32_t     g_l2logBuffPos;     /* Log Buffer block which is in use for L2 logging */
uint8_t      g_writeCirBuf;      /* Flag to indicate whether to write logs or not */
//Pst    g_rlog_pst;

#endif /* __RL_H__*/
