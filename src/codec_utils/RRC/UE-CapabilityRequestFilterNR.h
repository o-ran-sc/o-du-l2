/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_UE_CapabilityRequestFilterNR_H_
#define	_UE_CapabilityRequestFilterNR_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct FreqBandList;

/* UE-CapabilityRequestFilterNR */
typedef struct UE_CapabilityRequestFilterNR {
	struct FreqBandList	*frequencyBandListr;	/* OPTIONAL */
	struct UE_CapabilityRequestFilterNR__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UE_CapabilityRequestFilterNR_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UE_CapabilityRequestFilterNR;

#ifdef __cplusplus
}
#endif

#endif	/* _UE_CapabilityRequestFilterNR_H_ */
#include <asn_internal.h>
