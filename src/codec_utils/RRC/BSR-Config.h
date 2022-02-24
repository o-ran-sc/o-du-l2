/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_BSR_Config_H_
#define	_BSR_Config_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BSR_Config__periodicBSR_Timer {
	BSR_Config__periodicBSR_Timer_sf1	= 0,
	BSR_Config__periodicBSR_Timer_sf5	= 1,
	BSR_Config__periodicBSR_Timer_sf10	= 2,
	BSR_Config__periodicBSR_Timer_sf16	= 3,
	BSR_Config__periodicBSR_Timer_sf20	= 4,
	BSR_Config__periodicBSR_Timer_sf32	= 5,
	BSR_Config__periodicBSR_Timer_sf40	= 6,
	BSR_Config__periodicBSR_Timer_sf64	= 7,
	BSR_Config__periodicBSR_Timer_sf80	= 8,
	BSR_Config__periodicBSR_Timer_sf128	= 9,
	BSR_Config__periodicBSR_Timer_sf160	= 10,
	BSR_Config__periodicBSR_Timer_sf320	= 11,
	BSR_Config__periodicBSR_Timer_sf640	= 12,
	BSR_Config__periodicBSR_Timer_sf1280	= 13,
	BSR_Config__periodicBSR_Timer_sf2560	= 14,
	BSR_Config__periodicBSR_Timer_infinity	= 15
} e_BSR_Config__periodicBSR_Timer;
typedef enum BSR_Config__retxBSR_Timer {
	BSR_Config__retxBSR_Timer_sf10	= 0,
	BSR_Config__retxBSR_Timer_sf20	= 1,
	BSR_Config__retxBSR_Timer_sf40	= 2,
	BSR_Config__retxBSR_Timer_sf80	= 3,
	BSR_Config__retxBSR_Timer_sf160	= 4,
	BSR_Config__retxBSR_Timer_sf320	= 5,
	BSR_Config__retxBSR_Timer_sf640	= 6,
	BSR_Config__retxBSR_Timer_sf1280	= 7,
	BSR_Config__retxBSR_Timer_sf2560	= 8,
	BSR_Config__retxBSR_Timer_sf5120	= 9,
	BSR_Config__retxBSR_Timer_sf10240	= 10,
	BSR_Config__retxBSR_Timer_spare5	= 11,
	BSR_Config__retxBSR_Timer_spare4	= 12,
	BSR_Config__retxBSR_Timer_spare3	= 13,
	BSR_Config__retxBSR_Timer_spare2	= 14,
	BSR_Config__retxBSR_Timer_spare1	= 15
} e_BSR_Config__retxBSR_Timer;
typedef enum BSR_Config__logicalChannelSR_DelayTimer {
	BSR_Config__logicalChannelSR_DelayTimer_sf20	= 0,
	BSR_Config__logicalChannelSR_DelayTimer_sf40	= 1,
	BSR_Config__logicalChannelSR_DelayTimer_sf64	= 2,
	BSR_Config__logicalChannelSR_DelayTimer_sf128	= 3,
	BSR_Config__logicalChannelSR_DelayTimer_sf512	= 4,
	BSR_Config__logicalChannelSR_DelayTimer_sf1024	= 5,
	BSR_Config__logicalChannelSR_DelayTimer_sf2560	= 6,
	BSR_Config__logicalChannelSR_DelayTimer_spare1	= 7
} e_BSR_Config__logicalChannelSR_DelayTimer;

/* BSR-Config */
typedef struct BSR_Config {
	long	 periodicBSR_Timer;
	long	 retxBSR_Timer;
	long	*logicalChannelSR_DelayTimer;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BSR_Config_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_periodicBSR_Timer_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_retxBSR_Timer_19;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_logicalChannelSR_DelayTimer_36;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_BSR_Config;
extern asn_SEQUENCE_specifics_t asn_SPC_BSR_Config_specs_1;
extern asn_TYPE_member_t asn_MBR_BSR_Config_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _BSR_Config_H_ */
#include <asn_internal.h>
