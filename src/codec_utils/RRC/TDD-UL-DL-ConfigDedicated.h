/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_TDD_UL_DL_ConfigDedicated_H_
#define	_TDD_UL_DL_ConfigDedicated_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include "TDD-UL-DL-SlotIndex.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct TDD_UL_DL_SlotConfig;

/* TDD-UL-DL-ConfigDedicated */
typedef struct TDD_UL_DL_ConfigDedicated {
	struct TDD_UL_DL_ConfigDedicated__slotSpecificConfigurationsToAddModList {
		A_SEQUENCE_OF(struct TDD_UL_DL_SlotConfig) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *slotSpecificConfigurationsToAddModList;
	struct TDD_UL_DL_ConfigDedicated__slotSpecificConfigurationsToreleaseList {
		A_SEQUENCE_OF(TDD_UL_DL_SlotIndex_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *slotSpecificConfigurationsToreleaseList;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TDD_UL_DL_ConfigDedicated_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TDD_UL_DL_ConfigDedicated;
extern asn_SEQUENCE_specifics_t asn_SPC_TDD_UL_DL_ConfigDedicated_specs_1;
extern asn_TYPE_member_t asn_MBR_TDD_UL_DL_ConfigDedicated_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _TDD_UL_DL_ConfigDedicated_H_ */
#include <asn_internal.h>
