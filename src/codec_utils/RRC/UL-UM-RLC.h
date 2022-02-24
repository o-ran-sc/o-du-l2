/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_UL_UM_RLC_H_
#define	_UL_UM_RLC_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SN-FieldLengthUM.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UL-UM-RLC */
typedef struct UL_UM_RLC {
	SN_FieldLengthUM_t	*sn_FieldLength;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UL_UM_RLC_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UL_UM_RLC;
extern asn_SEQUENCE_specifics_t asn_SPC_UL_UM_RLC_specs_1;
extern asn_TYPE_member_t asn_MBR_UL_UM_RLC_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _UL_UM_RLC_H_ */
#include <asn_internal.h>
