/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SCellConfig_H_
#define	_SCellConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SCellIndex.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ServingCellConfigCommon;
struct ServingCellConfig;
struct SSB_MTC;

/* SCellConfig */
typedef struct SCellConfig {
	SCellIndex_t	 sCellIndex;
	struct ServingCellConfigCommon	*sCellConfigCommon;	/* OPTIONAL */
	struct ServingCellConfig	*sCellConfigDedicated;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct SCellConfig__ext1 {
		struct SSB_MTC	*smtc;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SCellConfig_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SCellConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_SCellConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_SCellConfig_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _SCellConfig_H_ */
#include <asn_internal.h>
