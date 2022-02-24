/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PUCCH_FormatConfig_H_
#define	_PUCCH_FormatConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include "PUCCH-MaxCodeRate.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PUCCH_FormatConfig__interslotFrequencyHopping {
	PUCCH_FormatConfig__interslotFrequencyHopping_enabled	= 0
} e_PUCCH_FormatConfig__interslotFrequencyHopping;
typedef enum PUCCH_FormatConfig__additionalDMRS {
	PUCCH_FormatConfig__additionalDMRS_true	= 0
} e_PUCCH_FormatConfig__additionalDMRS;
typedef enum PUCCH_FormatConfig__nrofSlots {
	PUCCH_FormatConfig__nrofSlots_n2	= 0,
	PUCCH_FormatConfig__nrofSlots_n4	= 1,
	PUCCH_FormatConfig__nrofSlots_n8	= 2
} e_PUCCH_FormatConfig__nrofSlots;
typedef enum PUCCH_FormatConfig__pi2BPSK {
	PUCCH_FormatConfig__pi2BPSK_enabled	= 0
} e_PUCCH_FormatConfig__pi2BPSK;
typedef enum PUCCH_FormatConfig__simultaneousHARQ_ACK_CSI {
	PUCCH_FormatConfig__simultaneousHARQ_ACK_CSI_true	= 0
} e_PUCCH_FormatConfig__simultaneousHARQ_ACK_CSI;

/* PUCCH-FormatConfig */
typedef struct PUCCH_FormatConfig {
	long	*interslotFrequencyHopping;	/* OPTIONAL */
	long	*additionalDMRS;	/* OPTIONAL */
	PUCCH_MaxCodeRate_t	*maxCodeRate;	/* OPTIONAL */
	long	*nrofSlots;	/* OPTIONAL */
	long	*pi2BPSK;	/* OPTIONAL */
	long	*simultaneousHARQ_ACK_CSI;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PUCCH_FormatConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_interslotFrequencyHopping_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_additionalDMRS_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_nrofSlots_7;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pi2BPSK_11;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_simultaneousHARQ_ACK_CSI_13;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PUCCH_FormatConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_PUCCH_FormatConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_PUCCH_FormatConfig_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _PUCCH_FormatConfig_H_ */
#include <asn_internal.h>
