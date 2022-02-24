/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MobilityFromNRCommand_IEs_H_
#define	_MobilityFromNRCommand_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MobilityFromNRCommand_IEs__targetRAT_Type {
	MobilityFromNRCommand_IEs__targetRAT_Type_eutra	= 0,
	MobilityFromNRCommand_IEs__targetRAT_Type_spare3	= 1,
	MobilityFromNRCommand_IEs__targetRAT_Type_spare2	= 2,
	MobilityFromNRCommand_IEs__targetRAT_Type_spare1	= 3
	/*
	 * Enumeration is extensible
	 */
} e_MobilityFromNRCommand_IEs__targetRAT_Type;

/* MobilityFromNRCommand-IEs */
typedef struct MobilityFromNRCommand_IEs {
	long	 targetRAT_Type;
	OCTET_STRING_t	 targetRAT_MessageContainer;
	OCTET_STRING_t	*nas_SecurityParamFromNR;	/* OPTIONAL */
	OCTET_STRING_t	*lateNonCriticalExtension;	/* OPTIONAL */
	struct MobilityFromNRCommand_IEs__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MobilityFromNRCommand_IEs_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_targetRAT_Type_2;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_MobilityFromNRCommand_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_MobilityFromNRCommand_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_MobilityFromNRCommand_IEs_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _MobilityFromNRCommand_IEs_H_ */
#include <asn_internal.h>
