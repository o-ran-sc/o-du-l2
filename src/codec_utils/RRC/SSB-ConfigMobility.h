/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_SSB_ConfigMobility_H_
#define	_SSB_ConfigMobility_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BOOLEAN.h>
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SSB_ConfigMobility__ssb_ToMeasure_PR {
	SSB_ConfigMobility__ssb_ToMeasure_PR_NOTHING,	/* No components present */
	SSB_ConfigMobility__ssb_ToMeasure_PR_release,
	SSB_ConfigMobility__ssb_ToMeasure_PR_setup
} SSB_ConfigMobility__ssb_ToMeasure_PR;

/* Forward declarations */
struct SS_RSSI_Measurement;
struct SSB_ToMeasure;

/* SSB-ConfigMobility */
typedef struct SSB_ConfigMobility {
	struct SSB_ConfigMobility__ssb_ToMeasure {
		SSB_ConfigMobility__ssb_ToMeasure_PR present;
		union SSB_ConfigMobility__ssb_ToMeasure_u {
			NULL_t	 release;
			struct SSB_ToMeasure	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ssb_ToMeasure;
	BOOLEAN_t	 deriveSSB_IndexFromCell;
	struct SS_RSSI_Measurement	*ss_RSSI_Measurement;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SSB_ConfigMobility_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SSB_ConfigMobility;
extern asn_SEQUENCE_specifics_t asn_SPC_SSB_ConfigMobility_specs_1;
extern asn_TYPE_member_t asn_MBR_SSB_ConfigMobility_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "SS-RSSI-Measurement.h"
#include "SSB-ToMeasure.h"

#endif	/* _SSB_ConfigMobility_H_ */
#include <asn_internal.h>
