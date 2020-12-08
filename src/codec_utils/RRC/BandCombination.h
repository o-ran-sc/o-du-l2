/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_BandCombination_H_
#define	_BandCombination_H_


#include <asn_application.h>

/* Including external dependencies */
#include "FeatureSetCombinationId.h"
#include <BIT_STRING.h>
#include <NativeEnumerated.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BandCombination__powerClass_v1530 {
	BandCombination__powerClass_v1530_pc2	= 0
} e_BandCombination__powerClass_v1530;

/* Forward declarations */
struct CA_ParametersEUTRA;
struct CA_ParametersNR;
struct MRDC_Parameters;
struct BandParameters;

/* BandCombination */
typedef struct BandCombination {
	struct BandCombination__bandList {
		A_SEQUENCE_OF(struct BandParameters) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} bandList;
	FeatureSetCombinationId_t	 featureSetCombination;
	struct CA_ParametersEUTRA	*ca_ParametersEUTRA;	/* OPTIONAL */
	struct CA_ParametersNR	*ca_ParametersNR;	/* OPTIONAL */
	struct MRDC_Parameters	*mrdc_Parameters;	/* OPTIONAL */
	BIT_STRING_t	*supportedBandwidthCombinationSet;	/* OPTIONAL */
	long	*powerClass_v1530;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BandCombination_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_powerClass_v1530_9;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_BandCombination;
extern asn_SEQUENCE_specifics_t asn_SPC_BandCombination_specs_1;
extern asn_TYPE_member_t asn_MBR_BandCombination_1[7];

#ifdef __cplusplus
}
#endif

#endif	/* _BandCombination_H_ */
#include <asn_internal.h>
