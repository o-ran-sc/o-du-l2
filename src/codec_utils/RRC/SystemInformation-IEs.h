/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SystemInformation_IEs_H_
#define	_SystemInformation_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SystemInformation_IEs__sib_TypeAndInfo__Member_PR {
	SystemInformation_IEs__sib_TypeAndInfo__Member_PR_NOTHING,	/* No components present */
	SystemInformation_IEs__sib_TypeAndInfo__Member_PR_sib2,
	SystemInformation_IEs__sib_TypeAndInfo__Member_PR_sib3,
	SystemInformation_IEs__sib_TypeAndInfo__Member_PR_sib4,
	SystemInformation_IEs__sib_TypeAndInfo__Member_PR_sib5,
	SystemInformation_IEs__sib_TypeAndInfo__Member_PR_sib6,
	SystemInformation_IEs__sib_TypeAndInfo__Member_PR_sib7,
	SystemInformation_IEs__sib_TypeAndInfo__Member_PR_sib8,
	SystemInformation_IEs__sib_TypeAndInfo__Member_PR_sib9
	/* Extensions may appear below */
	
} SystemInformation_IEs__sib_TypeAndInfo__Member_PR;

/* Forward declarations */
struct SIB2;
struct SIB3;
struct SIB4;
struct SIB5;
struct SIB6;
struct SIB7;
struct SIB8;
struct SIB9;

/* Forward definitions */
typedef struct SystemInformation_IEs__sib_TypeAndInfo__Member {
	SystemInformation_IEs__sib_TypeAndInfo__Member_PR present;
	union SystemInformation_IEs__sib_TypeAndInfo__Member_u {
		struct SIB2	*sib2;
		struct SIB3	*sib3;
		struct SIB4	*sib4;
		struct SIB5	*sib5;
		struct SIB6	*sib6;
		struct SIB7	*sib7;
		struct SIB8	*sib8;
		struct SIB9	*sib9;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SystemInformation_IEs__sib_TypeAndInfo__Member;

/* SystemInformation-IEs */
typedef struct SystemInformation_IEs {
	struct SystemInformation_IEs__sib_TypeAndInfo {
		A_SEQUENCE_OF(SystemInformation_IEs__sib_TypeAndInfo__Member) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} sib_TypeAndInfo;
	OCTET_STRING_t	*lateNonCriticalExtension;	/* OPTIONAL */
	struct SystemInformation_IEs__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SystemInformation_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SystemInformation_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_SystemInformation_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_SystemInformation_IEs_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _SystemInformation_IEs_H_ */
#include <asn_internal.h>
