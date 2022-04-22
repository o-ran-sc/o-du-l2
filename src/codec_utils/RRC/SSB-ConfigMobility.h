/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
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

#endif	/* _SSB_ConfigMobility_H_ */
#include <asn_internal.h>
