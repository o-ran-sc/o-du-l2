/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_DUtoCURRCInformation_H_
#define	_DUtoCURRCInformation_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CellGroupConfig.h"
#include "MeasGapConfig.h"
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolExtensionContainer;

/* DUtoCURRCInformation */
typedef struct DUtoCURRCInformation {
	CellGroupConfig_t	 cellGroupConfig;
	MeasGapConfig_t	*measGapConfig;	/* OPTIONAL */
	OCTET_STRING_t	*requestedP_MaxFR1;	/* OPTIONAL */
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DUtoCURRCInformation_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DUtoCURRCInformation;
extern asn_SEQUENCE_specifics_t asn_SPC_DUtoCURRCInformation_specs_1;
extern asn_TYPE_member_t asn_MBR_DUtoCURRCInformation_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _DUtoCURRCInformation_H_ */
#include <asn_internal.h>
