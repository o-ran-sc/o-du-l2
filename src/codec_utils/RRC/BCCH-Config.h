/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_BCCH_Config_H_
#define	_BCCH_Config_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BCCH_Config__modificationPeriodCoeff {
	BCCH_Config__modificationPeriodCoeff_n2	= 0,
	BCCH_Config__modificationPeriodCoeff_n4	= 1,
	BCCH_Config__modificationPeriodCoeff_n8	= 2,
	BCCH_Config__modificationPeriodCoeff_n16	= 3
} e_BCCH_Config__modificationPeriodCoeff;

/* BCCH-Config */
typedef struct BCCH_Config {
	long	 modificationPeriodCoeff;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BCCH_Config_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_modificationPeriodCoeff_2;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_BCCH_Config;
extern asn_SEQUENCE_specifics_t asn_SPC_BCCH_Config_specs_1;
extern asn_TYPE_member_t asn_MBR_BCCH_Config_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _BCCH_Config_H_ */
#include <asn_internal.h>
