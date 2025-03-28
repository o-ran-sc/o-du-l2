/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_FreqBandInformationNR_H_
#define	_FreqBandInformationNR_H_


#include <asn_application.h>

/* Including external dependencies */
#include "FreqBandIndicatorNR.h"
#include "AggregatedBandwidth.h"
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* FreqBandInformationNR */
typedef struct FreqBandInformationNR {
	FreqBandIndicatorNR_t	 bandNR;
	AggregatedBandwidth_t	*maxBandwidthRequestedDL;	/* OPTIONAL */
	AggregatedBandwidth_t	*maxBandwidthRequestedUL;	/* OPTIONAL */
	long	*maxCarriersRequestedDL;	/* OPTIONAL */
	long	*maxCarriersRequestedUL;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FreqBandInformationNR_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FreqBandInformationNR;
extern asn_SEQUENCE_specifics_t asn_SPC_FreqBandInformationNR_specs_1;
extern asn_TYPE_member_t asn_MBR_FreqBandInformationNR_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _FreqBandInformationNR_H_ */
#include <asn_internal.h>
