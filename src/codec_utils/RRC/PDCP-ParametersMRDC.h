/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PDCP_ParametersMRDC_H_
#define	_PDCP_ParametersMRDC_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PDCP_ParametersMRDC__pdcp_DuplicationSplitSRB {
	PDCP_ParametersMRDC__pdcp_DuplicationSplitSRB_supported	= 0
} e_PDCP_ParametersMRDC__pdcp_DuplicationSplitSRB;
typedef enum PDCP_ParametersMRDC__pdcp_DuplicationSplitDRB {
	PDCP_ParametersMRDC__pdcp_DuplicationSplitDRB_supported	= 0
} e_PDCP_ParametersMRDC__pdcp_DuplicationSplitDRB;

/* PDCP-ParametersMRDC */
typedef struct PDCP_ParametersMRDC {
	long	*pdcp_DuplicationSplitSRB;	/* OPTIONAL */
	long	*pdcp_DuplicationSplitDRB;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PDCP_ParametersMRDC_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_pdcp_DuplicationSplitSRB_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pdcp_DuplicationSplitDRB_4;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PDCP_ParametersMRDC;
extern asn_SEQUENCE_specifics_t asn_SPC_PDCP_ParametersMRDC_specs_1;
extern asn_TYPE_member_t asn_MBR_PDCP_ParametersMRDC_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _PDCP_ParametersMRDC_H_ */
#include <asn_internal.h>
