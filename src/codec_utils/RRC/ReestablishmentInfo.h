/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_ReestablishmentInfo_H_
#define	_ReestablishmentInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PhysCellId.h"
#include "ShortMAC-I.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ReestabNCellInfoList;

/* ReestablishmentInfo */
typedef struct ReestablishmentInfo {
	PhysCellId_t	 sourcePhysCellId;
	ShortMAC_I_t	 targetCellShortMAC_I;
	struct ReestabNCellInfoList	*additionalReestabInfoList;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ReestablishmentInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ReestablishmentInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_ReestablishmentInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_ReestablishmentInfo_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _ReestablishmentInfo_H_ */
#include <asn_internal.h>
