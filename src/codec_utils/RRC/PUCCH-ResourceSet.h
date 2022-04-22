/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PUCCH_ResourceSet_H_
#define	_PUCCH_ResourceSet_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PUCCH-ResourceSetId.h"
#include <NativeInteger.h>
#include "PUCCH-ResourceId.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PUCCH-ResourceSet */
typedef struct PUCCH_ResourceSet {
	PUCCH_ResourceSetId_t	 pucch_ResourceSetId;
	struct PUCCH_ResourceSet__resourceList {
		A_SEQUENCE_OF(PUCCH_ResourceId_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} resourceList;
	long	*maxPayloadMinus1;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PUCCH_ResourceSet_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PUCCH_ResourceSet;
extern asn_SEQUENCE_specifics_t asn_SPC_PUCCH_ResourceSet_specs_1;
extern asn_TYPE_member_t asn_MBR_PUCCH_ResourceSet_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _PUCCH_ResourceSet_H_ */
#include <asn_internal.h>
