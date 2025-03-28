/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_MeasObjectToAddMod_H_
#define	_MeasObjectToAddMod_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MeasObjectId.h"
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MeasObjectToAddMod__measObject_PR {
	MeasObjectToAddMod__measObject_PR_NOTHING,	/* No components present */
	MeasObjectToAddMod__measObject_PR_measObjectNR,
	/* Extensions may appear below */
	MeasObjectToAddMod__measObject_PR_measObjectEUTRA
} MeasObjectToAddMod__measObject_PR;

/* Forward declarations */
struct MeasObjectNR;
struct MeasObjectEUTRA;

/* MeasObjectToAddMod */
typedef struct MeasObjectToAddMod {
	MeasObjectId_t	 measObjectId;
	struct MeasObjectToAddMod__measObject {
		MeasObjectToAddMod__measObject_PR present;
		union MeasObjectToAddMod__measObject_u {
			struct MeasObjectNR	*measObjectNR;
			/*
			 * This type is extensible,
			 * possible extensions are below.
			 */
			struct MeasObjectEUTRA	*measObjectEUTRA;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} measObject;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasObjectToAddMod_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasObjectToAddMod;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasObjectToAddMod_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasObjectToAddMod_1[2];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "MeasObjectNR.h"
#include "MeasObjectEUTRA.h"

#endif	/* _MeasObjectToAddMod_H_ */
#include <asn_internal.h>
