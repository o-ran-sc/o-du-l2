/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_DRB_CountInfo_H_
#define	_DRB_CountInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DRB-Identity.h"
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DRB-CountInfo */
typedef struct DRB_CountInfo {
	DRB_Identity_t	 drb_Identity;
	unsigned long	 count_Uplink;
	unsigned long	 count_Downlink;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DRB_CountInfo_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_count_Uplink_3;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_count_Downlink_4;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_DRB_CountInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_DRB_CountInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_DRB_CountInfo_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _DRB_CountInfo_H_ */
#include <asn_internal.h>
