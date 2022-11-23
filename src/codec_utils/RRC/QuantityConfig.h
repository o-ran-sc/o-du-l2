/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_QuantityConfig_H_
#define	_QuantityConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct QuantityConfigNR;
struct FilterConfig;

/* QuantityConfig */
typedef struct QuantityConfig {
	struct QuantityConfig__quantityConfigNR_List {
		A_SEQUENCE_OF(struct QuantityConfigNR) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *quantityConfigNR_List;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct QuantityConfig__ext1 {
		struct FilterConfig	*quantityConfigEUTRA;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} QuantityConfig_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_QuantityConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_QuantityConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_QuantityConfig_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _QuantityConfig_H_ */
#include <asn_internal.h>
