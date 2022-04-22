/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RateMatchPatternLTE_CRS_H_
#define	_RateMatchPatternLTE_CRS_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RateMatchPatternLTE_CRS__carrierBandwidthDL {
	RateMatchPatternLTE_CRS__carrierBandwidthDL_n6	= 0,
	RateMatchPatternLTE_CRS__carrierBandwidthDL_n15	= 1,
	RateMatchPatternLTE_CRS__carrierBandwidthDL_n25	= 2,
	RateMatchPatternLTE_CRS__carrierBandwidthDL_n50	= 3,
	RateMatchPatternLTE_CRS__carrierBandwidthDL_n75	= 4,
	RateMatchPatternLTE_CRS__carrierBandwidthDL_n100	= 5,
	RateMatchPatternLTE_CRS__carrierBandwidthDL_spare2	= 6,
	RateMatchPatternLTE_CRS__carrierBandwidthDL_spare1	= 7
} e_RateMatchPatternLTE_CRS__carrierBandwidthDL;
typedef enum RateMatchPatternLTE_CRS__nrofCRS_Ports {
	RateMatchPatternLTE_CRS__nrofCRS_Ports_n1	= 0,
	RateMatchPatternLTE_CRS__nrofCRS_Ports_n2	= 1,
	RateMatchPatternLTE_CRS__nrofCRS_Ports_n4	= 2
} e_RateMatchPatternLTE_CRS__nrofCRS_Ports;
typedef enum RateMatchPatternLTE_CRS__v_Shift {
	RateMatchPatternLTE_CRS__v_Shift_n0	= 0,
	RateMatchPatternLTE_CRS__v_Shift_n1	= 1,
	RateMatchPatternLTE_CRS__v_Shift_n2	= 2,
	RateMatchPatternLTE_CRS__v_Shift_n3	= 3,
	RateMatchPatternLTE_CRS__v_Shift_n4	= 4,
	RateMatchPatternLTE_CRS__v_Shift_n5	= 5
} e_RateMatchPatternLTE_CRS__v_Shift;

/* Forward declarations */
struct EUTRA_MBSFN_SubframeConfigList;

/* RateMatchPatternLTE-CRS */
typedef struct RateMatchPatternLTE_CRS {
	long	 carrierFreqDL;
	long	 carrierBandwidthDL;
	struct EUTRA_MBSFN_SubframeConfigList	*mbsfn_SubframeConfigList;	/* OPTIONAL */
	long	 nrofCRS_Ports;
	long	 v_Shift;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RateMatchPatternLTE_CRS_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_carrierBandwidthDL_3;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_nrofCRS_Ports_13;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_v_Shift_17;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_RateMatchPatternLTE_CRS;
extern asn_SEQUENCE_specifics_t asn_SPC_RateMatchPatternLTE_CRS_specs_1;
extern asn_TYPE_member_t asn_MBR_RateMatchPatternLTE_CRS_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _RateMatchPatternLTE_CRS_H_ */
#include <asn_internal.h>
