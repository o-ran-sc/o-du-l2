/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_OverheatingAssistance_H_
#define	_OverheatingAssistance_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>
#include "ReducedAggregatedBandwidth.h"
#include "MIMO-LayersDL.h"
#include "MIMO-LayersUL.h"

#ifdef __cplusplus
extern "C" {
#endif

/* OverheatingAssistance */
typedef struct OverheatingAssistance {
	struct OverheatingAssistance__reducedMaxCCs {
		long	 reducedCCsDL;
		long	 reducedCCsUL;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *reducedMaxCCs;
	struct OverheatingAssistance__reducedMaxBW_FR1 {
		ReducedAggregatedBandwidth_t	 reducedBW_FR1_DL;
		ReducedAggregatedBandwidth_t	 reducedBW_FR1_UL;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *reducedMaxBW_FR1;
	struct OverheatingAssistance__reducedMaxBW_FR2 {
		ReducedAggregatedBandwidth_t	 reducedBW_FR2_DL;
		ReducedAggregatedBandwidth_t	 reducedBW_FR2_UL;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *reducedMaxBW_FR2;
	struct OverheatingAssistance__reducedMaxMIMO_LayersFR1 {
		MIMO_LayersDL_t	 reducedMIMO_LayersFR1_DL;
		MIMO_LayersUL_t	 reducedMIMO_LayersFR1_UL;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *reducedMaxMIMO_LayersFR1;
	struct OverheatingAssistance__reducedMaxMIMO_LayersFR2 {
		MIMO_LayersDL_t	 reducedMIMO_LayersFR2_DL;
		MIMO_LayersUL_t	 reducedMIMO_LayersFR2_UL;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *reducedMaxMIMO_LayersFR2;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} OverheatingAssistance_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_OverheatingAssistance;
extern asn_SEQUENCE_specifics_t asn_SPC_OverheatingAssistance_specs_1;
extern asn_TYPE_member_t asn_MBR_OverheatingAssistance_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _OverheatingAssistance_H_ */
#include <asn_internal.h>
