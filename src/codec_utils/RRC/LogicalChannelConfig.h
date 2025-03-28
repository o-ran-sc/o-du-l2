/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_LogicalChannelConfig_H_
#define	_LogicalChannelConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <NativeEnumerated.h>
#include "SchedulingRequestId.h"
#include <BOOLEAN.h>
#include "ServCellIndexRrc.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include "SubcarrierSpacing.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate {
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps0	= 0,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps8	= 1,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps16	= 2,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps32	= 3,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps64	= 4,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps128	= 5,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps256	= 6,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps512	= 7,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps1024	= 8,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps2048	= 9,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps4096	= 10,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps8192	= 11,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps16384	= 12,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps32768	= 13,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_kBps65536	= 14,
	LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_infinity	= 15
} e_LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate;
typedef enum LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration {
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_ms5	= 0,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_ms10	= 1,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_ms20	= 2,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_ms50	= 3,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_ms100	= 4,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_ms150	= 5,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_ms300	= 6,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_ms500	= 7,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_ms1000	= 8,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_spare7	= 9,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_spare6	= 10,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_spare5	= 11,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_spare4	= 12,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_spare3	= 13,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_spare2	= 14,
	LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration_spare1	= 15
} e_LogicalChannelConfig__ul_SpecificParameters__bucketSizeDuration;
typedef enum LogicalChannelConfig__ul_SpecificParameters__maxPUSCH_Duration {
	LogicalChannelConfig__ul_SpecificParameters__maxPUSCH_Duration_ms0p02	= 0,
	LogicalChannelConfig__ul_SpecificParameters__maxPUSCH_Duration_ms0p04	= 1,
	LogicalChannelConfig__ul_SpecificParameters__maxPUSCH_Duration_ms0p0625	= 2,
	LogicalChannelConfig__ul_SpecificParameters__maxPUSCH_Duration_ms0p125	= 3,
	LogicalChannelConfig__ul_SpecificParameters__maxPUSCH_Duration_ms0p25	= 4,
	LogicalChannelConfig__ul_SpecificParameters__maxPUSCH_Duration_ms0p5	= 5,
	LogicalChannelConfig__ul_SpecificParameters__maxPUSCH_Duration_spare2	= 6,
	LogicalChannelConfig__ul_SpecificParameters__maxPUSCH_Duration_spare1	= 7
} e_LogicalChannelConfig__ul_SpecificParameters__maxPUSCH_Duration;
typedef enum LogicalChannelConfig__ul_SpecificParameters__configuredGrantType1Allowed {
	LogicalChannelConfig__ul_SpecificParameters__configuredGrantType1Allowed_true	= 0
} e_LogicalChannelConfig__ul_SpecificParameters__configuredGrantType1Allowed;
typedef enum LogicalChannelConfig__ul_SpecificParameters__bitRateQueryProhibitTimer {
	LogicalChannelConfig__ul_SpecificParameters__bitRateQueryProhibitTimer_s0	= 0,
	LogicalChannelConfig__ul_SpecificParameters__bitRateQueryProhibitTimer_s0dot4	= 1,
	LogicalChannelConfig__ul_SpecificParameters__bitRateQueryProhibitTimer_s0dot8	= 2,
	LogicalChannelConfig__ul_SpecificParameters__bitRateQueryProhibitTimer_s1dot6	= 3,
	LogicalChannelConfig__ul_SpecificParameters__bitRateQueryProhibitTimer_s3	= 4,
	LogicalChannelConfig__ul_SpecificParameters__bitRateQueryProhibitTimer_s6	= 5,
	LogicalChannelConfig__ul_SpecificParameters__bitRateQueryProhibitTimer_s12	= 6,
	LogicalChannelConfig__ul_SpecificParameters__bitRateQueryProhibitTimer_s30	= 7
} e_LogicalChannelConfig__ul_SpecificParameters__bitRateQueryProhibitTimer;

/* LogicalChannelConfig */
typedef struct LogicalChannelConfig {
	struct LogicalChannelConfig__ul_SpecificParameters {
		long	 priority;
		long	 prioritisedBitRate;
		long	 bucketSizeDuration;
		struct LogicalChannelConfig__ul_SpecificParameters__allowedServingCells {
			A_SEQUENCE_OF(ServCellIndexRrc_t) list;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *allowedServingCells;
		struct LogicalChannelConfig__ul_SpecificParameters__allowedSCS_List {
			A_SEQUENCE_OF(SubcarrierSpacing_t) list;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *allowedSCS_List;
		long	*maxPUSCH_Duration;	/* OPTIONAL */
		long	*configuredGrantType1Allowed;	/* OPTIONAL */
		long	*logicalChannelGroup;	/* OPTIONAL */
		SchedulingRequestId_t	*schedulingRequestID;	/* OPTIONAL */
		BOOLEAN_t	 logicalChannelSR_Mask;
		BOOLEAN_t	 logicalChannelSR_DelayTimerApplied;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		long	*bitRateQueryProhibitTimer;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ul_SpecificParameters;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LogicalChannelConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_prioritisedBitRate_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_bucketSizeDuration_21;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_maxPUSCH_Duration_42;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_configuredGrantType1Allowed_51;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_bitRateQueryProhibitTimer_58;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_LogicalChannelConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_LogicalChannelConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_LogicalChannelConfig_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _LogicalChannelConfig_H_ */
#include <asn_internal.h>
