/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_SPS_Config_H_
#define	_SPS_Config_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include "PUCCH-ResourceId.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SPS_Config__periodicity {
	SPS_Config__periodicity_ms10	= 0,
	SPS_Config__periodicity_ms20	= 1,
	SPS_Config__periodicity_ms32	= 2,
	SPS_Config__periodicity_ms40	= 3,
	SPS_Config__periodicity_ms64	= 4,
	SPS_Config__periodicity_ms80	= 5,
	SPS_Config__periodicity_ms128	= 6,
	SPS_Config__periodicity_ms160	= 7,
	SPS_Config__periodicity_ms320	= 8,
	SPS_Config__periodicity_ms640	= 9,
	SPS_Config__periodicity_spare6	= 10,
	SPS_Config__periodicity_spare5	= 11,
	SPS_Config__periodicity_spare4	= 12,
	SPS_Config__periodicity_spare3	= 13,
	SPS_Config__periodicity_spare2	= 14,
	SPS_Config__periodicity_spare1	= 15
} e_SPS_Config__periodicity;
typedef enum SPS_Config__mcs_Table {
	SPS_Config__mcs_Table_qam64LowSE	= 0
} e_SPS_Config__mcs_Table;

/* SPS-Config */
typedef struct SPS_Config {
	long	 periodicity;
	long	 nrofHARQ_Processes;
	PUCCH_ResourceId_t	*n1PUCCH_AN;	/* OPTIONAL */
	long	*mcs_Table;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SPS_Config_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_periodicity_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_mcs_Table_21;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_SPS_Config;
extern asn_SEQUENCE_specifics_t asn_SPC_SPS_Config_specs_1;
extern asn_TYPE_member_t asn_MBR_SPS_Config_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _SPS_Config_H_ */
#include <asn_internal.h>
