/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_RRCSetupRequest_IEs_H_
#define	_RRCSetupRequest_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "InitialUE-Identity.h"
#include "EstablishmentCause.h"
#include <BIT_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RRCSetupRequest-IEs */
typedef struct RRCSetupRequest_IEs {
	InitialUE_Identity_t	 ue_Identity;
	EstablishmentCause_t	 establishmentCause;
	BIT_STRING_t	 spare;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRCSetupRequest_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RRCSetupRequest_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_RRCSetupRequest_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_RRCSetupRequest_IEs_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _RRCSetupRequest_IEs_H_ */
#include <asn_internal.h>
