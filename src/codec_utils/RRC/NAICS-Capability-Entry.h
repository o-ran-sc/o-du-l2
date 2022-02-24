/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_NAICS_Capability_Entry_H_
#define	_NAICS_Capability_Entry_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum NAICS_Capability_Entry__numberOfAggregatedPRB {
	NAICS_Capability_Entry__numberOfAggregatedPRB_n50	= 0,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n75	= 1,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n100	= 2,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n125	= 3,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n150	= 4,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n175	= 5,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n200	= 6,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n225	= 7,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n250	= 8,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n275	= 9,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n300	= 10,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n350	= 11,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n400	= 12,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n450	= 13,
	NAICS_Capability_Entry__numberOfAggregatedPRB_n500	= 14,
	NAICS_Capability_Entry__numberOfAggregatedPRB_spare	= 15
} e_NAICS_Capability_Entry__numberOfAggregatedPRB;

/* NAICS-Capability-Entry */
typedef struct NAICS_Capability_Entry {
	long	 numberOfNAICS_CapableCC;
	long	 numberOfAggregatedPRB;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} NAICS_Capability_Entry_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_numberOfAggregatedPRB_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_NAICS_Capability_Entry;
extern asn_SEQUENCE_specifics_t asn_SPC_NAICS_Capability_Entry_specs_1;
extern asn_TYPE_member_t asn_MBR_NAICS_Capability_Entry_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _NAICS_Capability_Entry_H_ */
#include <asn_internal.h>
