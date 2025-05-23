/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_DMRS_UplinkConfig_H_
#define	_DMRS_UplinkConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NULL.h>
#include <constr_CHOICE.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DMRS_UplinkConfig__dmrs_Type {
	DMRS_UplinkConfig__dmrs_Type_type2	= 0
} e_DMRS_UplinkConfig__dmrs_Type;
typedef enum DMRS_UplinkConfig__dmrs_AdditionalPosition {
	DMRS_UplinkConfig__dmrs_AdditionalPosition_pos0	= 0,
	DMRS_UplinkConfig__dmrs_AdditionalPosition_pos1	= 1,
	DMRS_UplinkConfig__dmrs_AdditionalPosition_pos3	= 2
} e_DMRS_UplinkConfig__dmrs_AdditionalPosition;
typedef enum DMRS_UplinkConfig__phaseTrackingRS_PR {
	DMRS_UplinkConfig__phaseTrackingRS_PR_NOTHING,	/* No components present */
	DMRS_UplinkConfig__phaseTrackingRS_PR_release,
	DMRS_UplinkConfig__phaseTrackingRS_PR_setup
} DMRS_UplinkConfig__phaseTrackingRS_PR;
typedef enum DMRS_UplinkConfig__maxLength {
	DMRS_UplinkConfig__maxLength_len2	= 0
} e_DMRS_UplinkConfig__maxLength;
typedef enum DMRS_UplinkConfig__transformPrecodingEnabled__sequenceGroupHopping {
	DMRS_UplinkConfig__transformPrecodingEnabled__sequenceGroupHopping_disabled	= 0
} e_DMRS_UplinkConfig__transformPrecodingEnabled__sequenceGroupHopping;
typedef enum DMRS_UplinkConfig__transformPrecodingEnabled__sequenceHopping {
	DMRS_UplinkConfig__transformPrecodingEnabled__sequenceHopping_enabled	= 0
} e_DMRS_UplinkConfig__transformPrecodingEnabled__sequenceHopping;

/* Forward declarations */
struct PTRS_UplinkConfig;

/* DMRS-UplinkConfig */
typedef struct DMRS_UplinkConfig {
	long	*dmrs_Type;	/* OPTIONAL */
	long	*dmrs_AdditionalPosition;	/* OPTIONAL */
	struct DMRS_UplinkConfig__phaseTrackingRS {
		DMRS_UplinkConfig__phaseTrackingRS_PR present;
		union DMRS_UplinkConfig__phaseTrackingRS_u {
			NULL_t	 release;
			struct PTRS_UplinkConfig	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *phaseTrackingRS;
	long	*maxLength;	/* OPTIONAL */
	struct DMRS_UplinkConfig__transformPrecodingDisabled {
		long	*scramblingID0;	/* OPTIONAL */
		long	*scramblingID1;	/* OPTIONAL */
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *transformPrecodingDisabled;
	struct DMRS_UplinkConfig__transformPrecodingEnabled {
		long	*nPUSCH_Identity;	/* OPTIONAL */
		long	*sequenceGroupHopping;	/* OPTIONAL */
		long	*sequenceHopping;	/* OPTIONAL */
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *transformPrecodingEnabled;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DMRS_UplinkConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_dmrs_Type_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_dmrs_AdditionalPosition_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_maxLength_11;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_sequenceGroupHopping_19;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_sequenceHopping_21;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_DMRS_UplinkConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_DMRS_UplinkConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_DMRS_UplinkConfig_1[6];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "PTRS-UplinkConfig.h"

#endif	/* _DMRS_UplinkConfig_H_ */
#include <asn_internal.h>
