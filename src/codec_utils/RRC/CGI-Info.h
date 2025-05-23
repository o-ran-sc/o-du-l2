/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_CGI_Info_H_
#define	_CGI_Info_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "PDCCH-ConfigSIB1.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct PLMN_IdentityInfoList;
struct MultiFrequencyBandListNR;

/* CGI-Info */
typedef struct CGI_Info {
	struct PLMN_IdentityInfoList	*plmn_IdentityInfoList;	/* OPTIONAL */
	struct MultiFrequencyBandListNR	*frequencyBandList;	/* OPTIONAL */
	struct CGI_Info__noSIB1 {
		long	 ssb_SubcarrierOffset;
		PDCCH_ConfigSIB1_t	 pdcch_ConfigSIB1;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *noSIB1;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CGI_Info_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CGI_Info;
extern asn_SEQUENCE_specifics_t asn_SPC_CGI_Info_specs_1;
extern asn_TYPE_member_t asn_MBR_CGI_Info_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "PLMN-IdentityInfoList.h"
#include "MultiFrequencyBandListNR.h"

#endif	/* _CGI_Info_H_ */
#include <asn_internal.h>
