/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PUCCH_TPC_CommandConfig_H_
#define	_PUCCH_TPC_CommandConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PUCCH-TPC-CommandConfig */
typedef struct PUCCH_TPC_CommandConfig {
	long	*tpc_IndexPCell;	/* OPTIONAL */
	long	*tpc_IndexPUCCH_SCell;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PUCCH_TPC_CommandConfig_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PUCCH_TPC_CommandConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_PUCCH_TPC_CommandConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_PUCCH_TPC_CommandConfig_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _PUCCH_TPC_CommandConfig_H_ */
#include <asn_internal.h>
