/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "/root/svaidhya/ASN1_file/E2APV0300.asn1"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2AP/`
 */

#ifndef	_RANfunctionIDcause_Item_H_
#define	_RANfunctionIDcause_Item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RANfunctionID.h"
#include "CauseE2.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RANfunctionIDcause-Item */
typedef struct RANfunctionIDcause_Item {
	RANfunctionID_t	 ranFunctionID;
	CauseE2_t	 cause;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RANfunctionIDcause_Item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RANfunctionIDcause_Item;
extern asn_SEQUENCE_specifics_t asn_SPC_RANfunctionIDcause_Item_specs_1;
extern asn_TYPE_member_t asn_MBR_RANfunctionIDcause_Item_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _RANfunctionIDcause_Item_H_ */
#include <asn_internal.h>
