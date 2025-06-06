/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_FR_Info_H_
#define	_FR_Info_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ServCellIndexRrc.h"
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum FR_Info__fr_Type {
	FR_Info__fr_Type_fr1	= 0,
	FR_Info__fr_Type_fr2	= 1
} e_FR_Info__fr_Type;

/* FR-Info */
typedef struct FR_Info {
	ServCellIndexRrc_t	 servCellIndex;
	long	 fr_Type;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FR_Info_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_fr_Type_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_FR_Info;
extern asn_SEQUENCE_specifics_t asn_SPC_FR_Info_specs_1;
extern asn_TYPE_member_t asn_MBR_FR_Info_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _FR_Info_H_ */
#include <asn_internal.h>
