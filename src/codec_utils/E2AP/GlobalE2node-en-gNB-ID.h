/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/E2APV0300.asn1"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2AP/`
 */

#ifndef	_GlobalE2node_en_gNB_ID_H_
#define	_GlobalE2node_en_gNB_ID_H_


#include <asn_application.h>

/* Including external dependencies */
#include "GlobalenGNB-ID.h"
#include "GNB-CU-UP-ID.h"
#include "GNB-DU-ID.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* GlobalE2node-en-gNB-ID */
typedef struct GlobalE2node_en_gNB_ID {
	GlobalenGNB_ID_t	 global_en_gNB_ID;
	GNB_CU_UP_ID_t	*en_gNB_CU_UP_ID;	/* OPTIONAL */
	GNB_DU_ID_t	*en_gNB_DU_ID;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} GlobalE2node_en_gNB_ID_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_GlobalE2node_en_gNB_ID;
extern asn_SEQUENCE_specifics_t asn_SPC_GlobalE2node_en_gNB_ID_specs_1;
extern asn_TYPE_member_t asn_MBR_GlobalE2node_en_gNB_ID_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _GlobalE2node_en_gNB_ID_H_ */
#include <asn_internal.h>
