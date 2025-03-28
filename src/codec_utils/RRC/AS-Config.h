/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_AS_Config_H_
#define	_AS_Config_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* AS-Config */
typedef struct AS_Config {
	OCTET_STRING_t	 rrcReconfiguration;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} AS_Config_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AS_Config;
extern asn_SEQUENCE_specifics_t asn_SPC_AS_Config_specs_1;
extern asn_TYPE_member_t asn_MBR_AS_Config_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _AS_Config_H_ */
#include <asn_internal.h>
