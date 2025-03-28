/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_SI_RequestResources_H_
#define	_SI_RequestResources_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SI-RequestResources */
typedef struct SI_RequestResources {
	long	 ra_PreambleStartIndex;
	long	*ra_AssociationPeriodIndex;	/* OPTIONAL */
	long	*ra_ssb_OccasionMaskIndex;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SI_RequestResources_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SI_RequestResources;
extern asn_SEQUENCE_specifics_t asn_SPC_SI_RequestResources_specs_1;
extern asn_TYPE_member_t asn_MBR_SI_RequestResources_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _SI_RequestResources_H_ */
#include <asn_internal.h>
