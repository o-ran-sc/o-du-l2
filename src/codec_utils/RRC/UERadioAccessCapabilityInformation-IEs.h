/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_UERadioAccessCapabilityInformation_IEs_H_
#define	_UERadioAccessCapabilityInformation_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UERadioAccessCapabilityInformation-IEs */
typedef struct UERadioAccessCapabilityInformation_IEs {
	OCTET_STRING_t	 ue_RadioAccessCapabilityInfo;
	struct UERadioAccessCapabilityInformation_IEs__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UERadioAccessCapabilityInformation_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UERadioAccessCapabilityInformation_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_UERadioAccessCapabilityInformation_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_UERadioAccessCapabilityInformation_IEs_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _UERadioAccessCapabilityInformation_IEs_H_ */
#include <asn_internal.h>
