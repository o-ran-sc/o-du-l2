/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_DLUPTNLInformation_ToBeSetup_Item_H_
#define	_DLUPTNLInformation_ToBeSetup_Item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "UPTransportLayerInformation.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolExtensionContainer;

/* DLUPTNLInformation-ToBeSetup-Item */
typedef struct DLUPTNLInformation_ToBeSetup_Item {
	UPTransportLayerInformation_t	 dLUPTNLInformation;
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DLUPTNLInformation_ToBeSetup_Item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DLUPTNLInformation_ToBeSetup_Item;
extern asn_SEQUENCE_specifics_t asn_SPC_DLUPTNLInformation_ToBeSetup_Item_specs_1;
extern asn_TYPE_member_t asn_MBR_DLUPTNLInformation_ToBeSetup_Item_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _DLUPTNLInformation_ToBeSetup_Item_H_ */
#include <asn_internal.h>
