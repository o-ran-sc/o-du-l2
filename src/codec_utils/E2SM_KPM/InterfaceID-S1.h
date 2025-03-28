/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-COMMON-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_SM_v2.01.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#ifndef	_InterfaceID_S1_H_
#define	_InterfaceID_S1_H_


#include <asn_application.h>

/* Including external dependencies */
#include "GUMMEI.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* InterfaceID-S1 */
typedef struct InterfaceID_S1 {
	GUMMEI_t	 gUMMEI;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} InterfaceID_S1_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_InterfaceID_S1;
extern asn_SEQUENCE_specifics_t asn_SPC_InterfaceID_S1_specs_1;
extern asn_TYPE_member_t asn_MBR_InterfaceID_S1_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _InterfaceID_S1_H_ */
#include <asn_internal.h>
