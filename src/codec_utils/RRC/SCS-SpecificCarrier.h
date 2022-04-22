/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SCS_SpecificCarrier_H_
#define	_SCS_SpecificCarrier_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "SubcarrierSpacing.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SCS-SpecificCarrier */
typedef struct SCS_SpecificCarrier {
	long	 offsetToCarrier;
	SubcarrierSpacing_t	 subcarrierSpacing;
	long	 carrierBandwidth;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct SCS_SpecificCarrier__ext1 {
		long	*txDirectCurrentLocation_v1530;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SCS_SpecificCarrier_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SCS_SpecificCarrier;
extern asn_SEQUENCE_specifics_t asn_SPC_SCS_SpecificCarrier_specs_1;
extern asn_TYPE_member_t asn_MBR_SCS_SpecificCarrier_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _SCS_SpecificCarrier_H_ */
#include <asn_internal.h>
