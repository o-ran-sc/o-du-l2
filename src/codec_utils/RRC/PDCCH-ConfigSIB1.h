/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PDCCH_ConfigSIB1_H_
#define	_PDCCH_ConfigSIB1_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ControlResourceSetZero.h"
#include "SearchSpaceZero.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PDCCH-ConfigSIB1 */
typedef struct PDCCH_ConfigSIB1 {
	ControlResourceSetZero_t	 controlResourceSetZero;
	SearchSpaceZero_t	 searchSpaceZero;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PDCCH_ConfigSIB1_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PDCCH_ConfigSIB1;
extern asn_SEQUENCE_specifics_t asn_SPC_PDCCH_ConfigSIB1_specs_1;
extern asn_TYPE_member_t asn_MBR_PDCCH_ConfigSIB1_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _PDCCH_ConfigSIB1_H_ */
#include <asn_internal.h>
