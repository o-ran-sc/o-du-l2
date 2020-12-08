/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_ReestabUE_Identity_H_
#define	_ReestabUE_Identity_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RNTI-Value.h"
#include "PhysCellId.h"
#include "ShortMAC-I.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ReestabUE-Identity */
typedef struct ReestabUE_Identity {
	RNTI_Value_t	 c_RNTI;
	PhysCellId_t	 physCellId;
	ShortMAC_I_t	 shortMAC_I;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ReestabUE_Identity_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ReestabUE_Identity;
extern asn_SEQUENCE_specifics_t asn_SPC_ReestabUE_Identity_specs_1;
extern asn_TYPE_member_t asn_MBR_ReestabUE_Identity_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _ReestabUE_Identity_H_ */
#include <asn_internal.h>
