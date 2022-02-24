/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
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
