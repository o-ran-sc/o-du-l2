/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/E2APV0300.asn1"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2AP/`
 */

#ifndef	_CriticalityDiagnosticsE2_IE_Item_H_
#define	_CriticalityDiagnosticsE2_IE_Item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CriticalityE2.h"
#include "ProtocolIE-IDE2.h"
#include "TypeOfError.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CriticalityDiagnosticsE2-IE-Item */
typedef struct CriticalityDiagnosticsE2_IE_Item {
	CriticalityE2_t	 iECriticality;
	ProtocolIE_IDE2_t	 iE_ID;
	TypeOfError_t	 typeOfError;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CriticalityDiagnosticsE2_IE_Item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CriticalityDiagnosticsE2_IE_Item;
extern asn_SEQUENCE_specifics_t asn_SPC_CriticalityDiagnosticsE2_IE_Item_specs_1;
extern asn_TYPE_member_t asn_MBR_CriticalityDiagnosticsE2_IE_Item_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _CriticalityDiagnosticsE2_IE_Item_H_ */
#include <asn_internal.h>
