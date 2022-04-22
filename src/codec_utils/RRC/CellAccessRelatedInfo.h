/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CellAccessRelatedInfo_H_
#define	_CellAccessRelatedInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PLMN-IdentityInfoList.h"
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CellAccessRelatedInfo__cellReservedForOtherUse {
	CellAccessRelatedInfo__cellReservedForOtherUse_true	= 0
} e_CellAccessRelatedInfo__cellReservedForOtherUse;

/* CellAccessRelatedInfo */
typedef struct CellAccessRelatedInfo {
	PLMN_IdentityInfoList_t	 plmn_IdentityList;
	long	*cellReservedForOtherUse;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CellAccessRelatedInfo_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_cellReservedForOtherUse_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_CellAccessRelatedInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_CellAccessRelatedInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_CellAccessRelatedInfo_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _CellAccessRelatedInfo_H_ */
#include <asn_internal.h>
