/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../mib_sib1.asn1"
 * 	`asn1c -D ./new_out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PLMN_IdentityInfo_H_
#define	_PLMN_IdentityInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TrackingAreaCode.h"
#include "RAN-AreaCode.h"
#include "CellIdentity.h"
#include <NativeEnumerated.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PLMN_IdentityInfo__cellReservedForOperatorUse {
	PLMN_IdentityInfo__cellReservedForOperatorUse_reserved	= 0,
	PLMN_IdentityInfo__cellReservedForOperatorUse_notReserved	= 1
} e_PLMN_IdentityInfo__cellReservedForOperatorUse;

/* Forward declarations */
struct PLMN_IdentitY;

/* PLMN-IdentityInfo */
typedef struct PLMN_IdentityInfo {
	struct PLMN_IdentityInfo__plmn_IdentityList {
		A_SEQUENCE_OF(struct PLMN_IdentitY) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} plmn_IdentityList;
	TrackingAreaCode_t	*trackingAreaCode;	/* OPTIONAL */
	RAN_AreaCode_t	*ranac;	/* OPTIONAL */
	CellIdentity_t	 cellIdentity;
	long	 cellReservedForOperatorUse;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PLMN_IdentityInfo_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_cellReservedForOperatorUse_7;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PLMN_IdentityInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_PLMN_IdentityInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_PLMN_IdentityInfo_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _PLMN_IdentityInfo_H_ */
#include <asn_internal.h>
