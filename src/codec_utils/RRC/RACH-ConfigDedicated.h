/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_RACH_ConfigDedicated_H_
#define	_RACH_ConfigDedicated_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CFRA;
struct RA_Prioritization;

/* RACH-ConfigDedicated */
typedef struct RACH_ConfigDedicated {
	struct CFRA	*cfra;	/* OPTIONAL */
	struct RA_Prioritization	*ra_Prioritization;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RACH_ConfigDedicated_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RACH_ConfigDedicated;
extern asn_SEQUENCE_specifics_t asn_SPC_RACH_ConfigDedicated_specs_1;
extern asn_TYPE_member_t asn_MBR_RACH_ConfigDedicated_1[2];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CFRA.h"
#include "RA-Prioritization.h"

#endif	/* _RACH_ConfigDedicated_H_ */
#include <asn_internal.h>
