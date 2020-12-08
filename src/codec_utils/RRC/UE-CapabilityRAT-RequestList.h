/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_UE_CapabilityRAT_RequestList_H_
#define	_UE_CapabilityRAT_RequestList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct UE_CapabilityRAT_Request;

/* UE-CapabilityRAT-RequestList */
typedef struct UE_CapabilityRAT_RequestList {
	A_SEQUENCE_OF(struct UE_CapabilityRAT_Request) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UE_CapabilityRAT_RequestList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UE_CapabilityRAT_RequestList;
extern asn_SET_OF_specifics_t asn_SPC_UE_CapabilityRAT_RequestList_specs_1;
extern asn_TYPE_member_t asn_MBR_UE_CapabilityRAT_RequestList_1[1];
extern asn_per_constraints_t asn_PER_type_UE_CapabilityRAT_RequestList_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _UE_CapabilityRAT_RequestList_H_ */
#include <asn_internal.h>
