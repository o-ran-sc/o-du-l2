/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_PCCH_Config_H_
#define	_PCCH_Config_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PagingCycle.h"
#include <NativeEnumerated.h>
#include <NULL.h>
#include <NativeInteger.h>
#include <constr_CHOICE.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PCCH_Config__nAndPagingFrameOffset_PR {
	PCCH_Config__nAndPagingFrameOffset_PR_NOTHING,	/* No components present */
	PCCH_Config__nAndPagingFrameOffset_PR_oneT,
	PCCH_Config__nAndPagingFrameOffset_PR_halfT,
	PCCH_Config__nAndPagingFrameOffset_PR_quarterT,
	PCCH_Config__nAndPagingFrameOffset_PR_oneEighthT,
	PCCH_Config__nAndPagingFrameOffset_PR_oneSixteenthT
} PCCH_Config__nAndPagingFrameOffset_PR;
typedef enum PCCH_Config__ns {
	PCCH_Config__ns_four	= 0,
	PCCH_Config__ns_two	= 1,
	PCCH_Config__ns_one	= 2
} e_PCCH_Config__ns;
typedef enum PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR {
	PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_NOTHING,	/* No components present */
	PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_sCS15KHZoneT,
	PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_sCS30KHZoneT_SCS15KHZhalfT,
	PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_sCS60KHZoneT_SCS30KHZhalfT_SCS15KHZquarterT,
	PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_sCS120KHZoneT_SCS60KHZhalfT_SCS30KHZquarterT_SCS15KHZoneEighthT,
	PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_sCS120KHZhalfT_SCS60KHZquarterT_SCS30KHZoneEighthT_SCS15KHZoneSixteenthT,
	PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_sCS120KHZquarterT_SCS60KHZoneEighthT_SCS30KHZoneSixteenthT,
	PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_sCS120KHZoneEighthT_SCS60KHZoneSixteenthT,
	PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_sCS120KHZoneSixteenthT
} PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR;

/* PCCH-Config */
typedef struct PCCH_Config {
	PagingCycle_t	 defaultPagingCycle;
	struct PCCH_Config__nAndPagingFrameOffset {
		PCCH_Config__nAndPagingFrameOffset_PR present;
		union PCCH_Config__nAndPagingFrameOffset_u {
			NULL_t	 oneT;
			long	 halfT;
			long	 quarterT;
			long	 oneEighthT;
			long	 oneSixteenthT;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} nAndPagingFrameOffset;
	long	 ns;
	struct PCCH_Config__firstPDCCH_MonitoringOccasionOfPO {
		PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR present;
		union PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_u {
			struct PCCH_Config__firstPDCCH_MonitoringOccasionOfPO__sCS15KHZoneT {
				A_SEQUENCE_OF(long) list;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *sCS15KHZoneT;
			struct PCCH_Config__firstPDCCH_MonitoringOccasionOfPO__sCS30KHZoneT_SCS15KHZhalfT {
				A_SEQUENCE_OF(long) list;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *sCS30KHZoneT_SCS15KHZhalfT;
			struct PCCH_Config__firstPDCCH_MonitoringOccasionOfPO__sCS60KHZoneT_SCS30KHZhalfT_SCS15KHZquarterT {
				A_SEQUENCE_OF(long) list;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *sCS60KHZoneT_SCS30KHZhalfT_SCS15KHZquarterT;
			struct PCCH_Config__firstPDCCH_MonitoringOccasionOfPO__sCS120KHZoneT_SCS60KHZhalfT_SCS30KHZquarterT_SCS15KHZoneEighthT {
				A_SEQUENCE_OF(long) list;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *sCS120KHZoneT_SCS60KHZhalfT_SCS30KHZquarterT_SCS15KHZoneEighthT;
			struct PCCH_Config__firstPDCCH_MonitoringOccasionOfPO__sCS120KHZhalfT_SCS60KHZquarterT_SCS30KHZoneEighthT_SCS15KHZoneSixteenthT {
				A_SEQUENCE_OF(long) list;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *sCS120KHZhalfT_SCS60KHZquarterT_SCS30KHZoneEighthT_SCS15KHZoneSixteenthT;
			struct PCCH_Config__firstPDCCH_MonitoringOccasionOfPO__sCS120KHZquarterT_SCS60KHZoneEighthT_SCS30KHZoneSixteenthT {
				A_SEQUENCE_OF(long) list;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *sCS120KHZquarterT_SCS60KHZoneEighthT_SCS30KHZoneSixteenthT;
			struct PCCH_Config__firstPDCCH_MonitoringOccasionOfPO__sCS120KHZoneEighthT_SCS60KHZoneSixteenthT {
				A_SEQUENCE_OF(long) list;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *sCS120KHZoneEighthT_SCS60KHZoneSixteenthT;
			struct PCCH_Config__firstPDCCH_MonitoringOccasionOfPO__sCS120KHZoneSixteenthT {
				A_SEQUENCE_OF(long) list;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *sCS120KHZoneSixteenthT;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *firstPDCCH_MonitoringOccasionOfPO;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PCCH_Config_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_ns_9;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PCCH_Config;
extern asn_SEQUENCE_specifics_t asn_SPC_PCCH_Config_specs_1;
extern asn_TYPE_member_t asn_MBR_PCCH_Config_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _PCCH_Config_H_ */
#include <asn_internal.h>
