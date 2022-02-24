/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_DRX_Config_H_
#define	_DRX_Config_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DRX_Config__drx_onDurationTimer_PR {
	DRX_Config__drx_onDurationTimer_PR_NOTHING,	/* No components present */
	DRX_Config__drx_onDurationTimer_PR_subMilliSeconds,
	DRX_Config__drx_onDurationTimer_PR_milliSeconds
} DRX_Config__drx_onDurationTimer_PR;
typedef enum DRX_Config__drx_onDurationTimer__milliSeconds {
	DRX_Config__drx_onDurationTimer__milliSeconds_ms1	= 0,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms2	= 1,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms3	= 2,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms4	= 3,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms5	= 4,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms6	= 5,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms8	= 6,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms10	= 7,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms20	= 8,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms30	= 9,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms40	= 10,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms50	= 11,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms60	= 12,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms80	= 13,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms100	= 14,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms200	= 15,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms300	= 16,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms400	= 17,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms500	= 18,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms600	= 19,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms800	= 20,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms1000	= 21,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms1200	= 22,
	DRX_Config__drx_onDurationTimer__milliSeconds_ms1600	= 23,
	DRX_Config__drx_onDurationTimer__milliSeconds_spare8	= 24,
	DRX_Config__drx_onDurationTimer__milliSeconds_spare7	= 25,
	DRX_Config__drx_onDurationTimer__milliSeconds_spare6	= 26,
	DRX_Config__drx_onDurationTimer__milliSeconds_spare5	= 27,
	DRX_Config__drx_onDurationTimer__milliSeconds_spare4	= 28,
	DRX_Config__drx_onDurationTimer__milliSeconds_spare3	= 29,
	DRX_Config__drx_onDurationTimer__milliSeconds_spare2	= 30,
	DRX_Config__drx_onDurationTimer__milliSeconds_spare1	= 31
} e_DRX_Config__drx_onDurationTimer__milliSeconds;
typedef enum DRX_Config__drx_InactivityTimer {
	DRX_Config__drx_InactivityTimer_ms0	= 0,
	DRX_Config__drx_InactivityTimer_ms1	= 1,
	DRX_Config__drx_InactivityTimer_ms2	= 2,
	DRX_Config__drx_InactivityTimer_ms3	= 3,
	DRX_Config__drx_InactivityTimer_ms4	= 4,
	DRX_Config__drx_InactivityTimer_ms5	= 5,
	DRX_Config__drx_InactivityTimer_ms6	= 6,
	DRX_Config__drx_InactivityTimer_ms8	= 7,
	DRX_Config__drx_InactivityTimer_ms10	= 8,
	DRX_Config__drx_InactivityTimer_ms20	= 9,
	DRX_Config__drx_InactivityTimer_ms30	= 10,
	DRX_Config__drx_InactivityTimer_ms40	= 11,
	DRX_Config__drx_InactivityTimer_ms50	= 12,
	DRX_Config__drx_InactivityTimer_ms60	= 13,
	DRX_Config__drx_InactivityTimer_ms80	= 14,
	DRX_Config__drx_InactivityTimer_ms100	= 15,
	DRX_Config__drx_InactivityTimer_ms200	= 16,
	DRX_Config__drx_InactivityTimer_ms300	= 17,
	DRX_Config__drx_InactivityTimer_ms500	= 18,
	DRX_Config__drx_InactivityTimer_ms750	= 19,
	DRX_Config__drx_InactivityTimer_ms1280	= 20,
	DRX_Config__drx_InactivityTimer_ms1920	= 21,
	DRX_Config__drx_InactivityTimer_ms2560	= 22,
	DRX_Config__drx_InactivityTimer_spare9	= 23,
	DRX_Config__drx_InactivityTimer_spare8	= 24,
	DRX_Config__drx_InactivityTimer_spare7	= 25,
	DRX_Config__drx_InactivityTimer_spare6	= 26,
	DRX_Config__drx_InactivityTimer_spare5	= 27,
	DRX_Config__drx_InactivityTimer_spare4	= 28,
	DRX_Config__drx_InactivityTimer_spare3	= 29,
	DRX_Config__drx_InactivityTimer_spare2	= 30,
	DRX_Config__drx_InactivityTimer_spare1	= 31
} e_DRX_Config__drx_InactivityTimer;
typedef enum DRX_Config__drx_RetransmissionTimerDL {
	DRX_Config__drx_RetransmissionTimerDL_sl0	= 0,
	DRX_Config__drx_RetransmissionTimerDL_sl1	= 1,
	DRX_Config__drx_RetransmissionTimerDL_sl2	= 2,
	DRX_Config__drx_RetransmissionTimerDL_sl4	= 3,
	DRX_Config__drx_RetransmissionTimerDL_sl6	= 4,
	DRX_Config__drx_RetransmissionTimerDL_sl8	= 5,
	DRX_Config__drx_RetransmissionTimerDL_sl16	= 6,
	DRX_Config__drx_RetransmissionTimerDL_sl24	= 7,
	DRX_Config__drx_RetransmissionTimerDL_sl33	= 8,
	DRX_Config__drx_RetransmissionTimerDL_sl40	= 9,
	DRX_Config__drx_RetransmissionTimerDL_sl64	= 10,
	DRX_Config__drx_RetransmissionTimerDL_sl80	= 11,
	DRX_Config__drx_RetransmissionTimerDL_sl96	= 12,
	DRX_Config__drx_RetransmissionTimerDL_sl112	= 13,
	DRX_Config__drx_RetransmissionTimerDL_sl128	= 14,
	DRX_Config__drx_RetransmissionTimerDL_sl160	= 15,
	DRX_Config__drx_RetransmissionTimerDL_sl320	= 16,
	DRX_Config__drx_RetransmissionTimerDL_spare15	= 17,
	DRX_Config__drx_RetransmissionTimerDL_spare14	= 18,
	DRX_Config__drx_RetransmissionTimerDL_spare13	= 19,
	DRX_Config__drx_RetransmissionTimerDL_spare12	= 20,
	DRX_Config__drx_RetransmissionTimerDL_spare11	= 21,
	DRX_Config__drx_RetransmissionTimerDL_spare10	= 22,
	DRX_Config__drx_RetransmissionTimerDL_spare9	= 23,
	DRX_Config__drx_RetransmissionTimerDL_spare8	= 24,
	DRX_Config__drx_RetransmissionTimerDL_spare7	= 25,
	DRX_Config__drx_RetransmissionTimerDL_spare6	= 26,
	DRX_Config__drx_RetransmissionTimerDL_spare5	= 27,
	DRX_Config__drx_RetransmissionTimerDL_spare4	= 28,
	DRX_Config__drx_RetransmissionTimerDL_spare3	= 29,
	DRX_Config__drx_RetransmissionTimerDL_spare2	= 30,
	DRX_Config__drx_RetransmissionTimerDL_spare1	= 31
} e_DRX_Config__drx_RetransmissionTimerDL;
typedef enum DRX_Config__drx_RetransmissionTimerUL {
	DRX_Config__drx_RetransmissionTimerUL_sl0	= 0,
	DRX_Config__drx_RetransmissionTimerUL_sl1	= 1,
	DRX_Config__drx_RetransmissionTimerUL_sl2	= 2,
	DRX_Config__drx_RetransmissionTimerUL_sl4	= 3,
	DRX_Config__drx_RetransmissionTimerUL_sl6	= 4,
	DRX_Config__drx_RetransmissionTimerUL_sl8	= 5,
	DRX_Config__drx_RetransmissionTimerUL_sl16	= 6,
	DRX_Config__drx_RetransmissionTimerUL_sl24	= 7,
	DRX_Config__drx_RetransmissionTimerUL_sl33	= 8,
	DRX_Config__drx_RetransmissionTimerUL_sl40	= 9,
	DRX_Config__drx_RetransmissionTimerUL_sl64	= 10,
	DRX_Config__drx_RetransmissionTimerUL_sl80	= 11,
	DRX_Config__drx_RetransmissionTimerUL_sl96	= 12,
	DRX_Config__drx_RetransmissionTimerUL_sl112	= 13,
	DRX_Config__drx_RetransmissionTimerUL_sl128	= 14,
	DRX_Config__drx_RetransmissionTimerUL_sl160	= 15,
	DRX_Config__drx_RetransmissionTimerUL_sl320	= 16,
	DRX_Config__drx_RetransmissionTimerUL_spare15	= 17,
	DRX_Config__drx_RetransmissionTimerUL_spare14	= 18,
	DRX_Config__drx_RetransmissionTimerUL_spare13	= 19,
	DRX_Config__drx_RetransmissionTimerUL_spare12	= 20,
	DRX_Config__drx_RetransmissionTimerUL_spare11	= 21,
	DRX_Config__drx_RetransmissionTimerUL_spare10	= 22,
	DRX_Config__drx_RetransmissionTimerUL_spare9	= 23,
	DRX_Config__drx_RetransmissionTimerUL_spare8	= 24,
	DRX_Config__drx_RetransmissionTimerUL_spare7	= 25,
	DRX_Config__drx_RetransmissionTimerUL_spare6	= 26,
	DRX_Config__drx_RetransmissionTimerUL_spare5	= 27,
	DRX_Config__drx_RetransmissionTimerUL_spare4	= 28,
	DRX_Config__drx_RetransmissionTimerUL_spare3	= 29,
	DRX_Config__drx_RetransmissionTimerUL_spare2	= 30,
	DRX_Config__drx_RetransmissionTimerUL_spare1	= 31
} e_DRX_Config__drx_RetransmissionTimerUL;
typedef enum DRX_Config__drx_LongCycleStartOffset_PR {
	DRX_Config__drx_LongCycleStartOffset_PR_NOTHING,	/* No components present */
	DRX_Config__drx_LongCycleStartOffset_PR_ms10,
	DRX_Config__drx_LongCycleStartOffset_PR_ms20,
	DRX_Config__drx_LongCycleStartOffset_PR_ms32,
	DRX_Config__drx_LongCycleStartOffset_PR_ms40,
	DRX_Config__drx_LongCycleStartOffset_PR_ms60,
	DRX_Config__drx_LongCycleStartOffset_PR_ms64,
	DRX_Config__drx_LongCycleStartOffset_PR_ms70,
	DRX_Config__drx_LongCycleStartOffset_PR_ms80,
	DRX_Config__drx_LongCycleStartOffset_PR_ms128,
	DRX_Config__drx_LongCycleStartOffset_PR_ms160,
	DRX_Config__drx_LongCycleStartOffset_PR_ms256,
	DRX_Config__drx_LongCycleStartOffset_PR_ms320,
	DRX_Config__drx_LongCycleStartOffset_PR_ms512,
	DRX_Config__drx_LongCycleStartOffset_PR_ms640,
	DRX_Config__drx_LongCycleStartOffset_PR_ms1024,
	DRX_Config__drx_LongCycleStartOffset_PR_ms1280,
	DRX_Config__drx_LongCycleStartOffset_PR_ms2048,
	DRX_Config__drx_LongCycleStartOffset_PR_ms2560,
	DRX_Config__drx_LongCycleStartOffset_PR_ms5120,
	DRX_Config__drx_LongCycleStartOffset_PR_ms10240
} DRX_Config__drx_LongCycleStartOffset_PR;
typedef enum DRX_Config__shortDRX__drx_ShortCycle {
	DRX_Config__shortDRX__drx_ShortCycle_ms2	= 0,
	DRX_Config__shortDRX__drx_ShortCycle_ms3	= 1,
	DRX_Config__shortDRX__drx_ShortCycle_ms4	= 2,
	DRX_Config__shortDRX__drx_ShortCycle_ms5	= 3,
	DRX_Config__shortDRX__drx_ShortCycle_ms6	= 4,
	DRX_Config__shortDRX__drx_ShortCycle_ms7	= 5,
	DRX_Config__shortDRX__drx_ShortCycle_ms8	= 6,
	DRX_Config__shortDRX__drx_ShortCycle_ms10	= 7,
	DRX_Config__shortDRX__drx_ShortCycle_ms14	= 8,
	DRX_Config__shortDRX__drx_ShortCycle_ms16	= 9,
	DRX_Config__shortDRX__drx_ShortCycle_ms20	= 10,
	DRX_Config__shortDRX__drx_ShortCycle_ms30	= 11,
	DRX_Config__shortDRX__drx_ShortCycle_ms32	= 12,
	DRX_Config__shortDRX__drx_ShortCycle_ms35	= 13,
	DRX_Config__shortDRX__drx_ShortCycle_ms40	= 14,
	DRX_Config__shortDRX__drx_ShortCycle_ms64	= 15,
	DRX_Config__shortDRX__drx_ShortCycle_ms80	= 16,
	DRX_Config__shortDRX__drx_ShortCycle_ms128	= 17,
	DRX_Config__shortDRX__drx_ShortCycle_ms160	= 18,
	DRX_Config__shortDRX__drx_ShortCycle_ms256	= 19,
	DRX_Config__shortDRX__drx_ShortCycle_ms320	= 20,
	DRX_Config__shortDRX__drx_ShortCycle_ms512	= 21,
	DRX_Config__shortDRX__drx_ShortCycle_ms640	= 22,
	DRX_Config__shortDRX__drx_ShortCycle_spare9	= 23,
	DRX_Config__shortDRX__drx_ShortCycle_spare8	= 24,
	DRX_Config__shortDRX__drx_ShortCycle_spare7	= 25,
	DRX_Config__shortDRX__drx_ShortCycle_spare6	= 26,
	DRX_Config__shortDRX__drx_ShortCycle_spare5	= 27,
	DRX_Config__shortDRX__drx_ShortCycle_spare4	= 28,
	DRX_Config__shortDRX__drx_ShortCycle_spare3	= 29,
	DRX_Config__shortDRX__drx_ShortCycle_spare2	= 30,
	DRX_Config__shortDRX__drx_ShortCycle_spare1	= 31
} e_DRX_Config__shortDRX__drx_ShortCycle;

/* DRX-Config */
typedef struct DRX_Config {
	struct DRX_Config__drx_onDurationTimer {
		DRX_Config__drx_onDurationTimer_PR present;
		union DRX_Config__drx_onDurationTimer_u {
			long	 subMilliSeconds;
			long	 milliSeconds;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} drx_onDurationTimer;
	long	 drx_InactivityTimer;
	long	 drx_HARQ_RTT_TimerDL;
	long	 drx_HARQ_RTT_TimerUL;
	long	 drx_RetransmissionTimerDL;
	long	 drx_RetransmissionTimerUL;
	struct DRX_Config__drx_LongCycleStartOffset {
		DRX_Config__drx_LongCycleStartOffset_PR present;
		union DRX_Config__drx_LongCycleStartOffset_u {
			long	 ms10;
			long	 ms20;
			long	 ms32;
			long	 ms40;
			long	 ms60;
			long	 ms64;
			long	 ms70;
			long	 ms80;
			long	 ms128;
			long	 ms160;
			long	 ms256;
			long	 ms320;
			long	 ms512;
			long	 ms640;
			long	 ms1024;
			long	 ms1280;
			long	 ms2048;
			long	 ms2560;
			long	 ms5120;
			long	 ms10240;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} drx_LongCycleStartOffset;
	struct DRX_Config__shortDRX {
		long	 drx_ShortCycle;
		long	 drx_ShortCycleTimer;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *shortDRX;
	long	 drx_SlotOffset;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DRX_Config_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_milliSeconds_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_drx_InactivityTimer_37;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_drx_RetransmissionTimerDL_72;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_drx_RetransmissionTimerUL_105;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_drx_ShortCycle_160;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_DRX_Config;
extern asn_SEQUENCE_specifics_t asn_SPC_DRX_Config_specs_1;
extern asn_TYPE_member_t asn_MBR_DRX_Config_1[9];

#ifdef __cplusplus
}
#endif

#endif	/* _DRX_Config_H_ */
#include <asn_internal.h>
