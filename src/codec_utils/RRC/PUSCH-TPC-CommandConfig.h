/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_PUSCH_TPC_CommandConfig_H_
#define	_PUSCH_TPC_CommandConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "ServCellIndexRrc.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PUSCH-TPC-CommandConfig */
typedef struct PUSCH_TPC_CommandConfig {
	long	*tpc_Index;	/* OPTIONAL */
	long	*tpc_IndexSUL;	/* OPTIONAL */
	ServCellIndexRrc_t	*targetCell;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PUSCH_TPC_CommandConfig_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PUSCH_TPC_CommandConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_PUSCH_TPC_CommandConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_PUSCH_TPC_CommandConfig_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _PUSCH_TPC_CommandConfig_H_ */
#include <asn_internal.h>
