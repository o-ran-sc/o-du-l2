/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SecurityModeFailure_IEs_H_
#define	_SecurityModeFailure_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SecurityModeFailure-IEs */
typedef struct SecurityModeFailure_IEs {
	OCTET_STRING_t	*lateNonCriticalExtension;	/* OPTIONAL */
	struct SecurityModeFailure_IEs__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SecurityModeFailure_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SecurityModeFailure_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_SecurityModeFailure_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_SecurityModeFailure_IEs_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _SecurityModeFailure_IEs_H_ */
#include <asn_internal.h>
