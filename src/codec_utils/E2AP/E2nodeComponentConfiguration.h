/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_E2nodeComponentConfiguration_H_
#define	_E2nodeComponentConfiguration_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* E2nodeComponentConfiguration */
typedef struct E2nodeComponentConfiguration {
	OCTET_STRING_t	 e2nodeComponentRequestPart;
	OCTET_STRING_t	 e2nodeComponentResponsePart;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} E2nodeComponentConfiguration_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_E2nodeComponentConfiguration;
extern asn_SEQUENCE_specifics_t asn_SPC_E2nodeComponentConfiguration_specs_1;
extern asn_TYPE_member_t asn_MBR_E2nodeComponentConfiguration_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _E2nodeComponentConfiguration_H_ */
#include <asn_internal.h>
