/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../mib_sib1.asn1"
 * 	`asn1c -D ./new_out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PUCCH_ConfigCommon_H_
#define	_PUCCH_ConfigCommon_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PUCCH_ConfigCommon__pucch_GroupHopping {
	PUCCH_ConfigCommon__pucch_GroupHopping_neither	= 0,
	PUCCH_ConfigCommon__pucch_GroupHopping_enable	= 1,
	PUCCH_ConfigCommon__pucch_GroupHopping_disable	= 2
} e_PUCCH_ConfigCommon__pucch_GroupHopping;

/* PUCCH-ConfigCommon */
typedef struct PUCCH_ConfigCommon {
	long	*pucch_ResourceCommon;	/* OPTIONAL */
	long	 pucch_GroupHopping;
	long	*hoppingId;	/* OPTIONAL */
	long	*p0_nominal;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PUCCH_ConfigCommon_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_pucch_GroupHopping_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PUCCH_ConfigCommon;
extern asn_SEQUENCE_specifics_t asn_SPC_PUCCH_ConfigCommon_specs_1;
extern asn_TYPE_member_t asn_MBR_PUCCH_ConfigCommon_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _PUCCH_ConfigCommon_H_ */
#include <asn_internal.h>
