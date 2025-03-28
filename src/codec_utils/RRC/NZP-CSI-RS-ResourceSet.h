/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_NZP_CSI_RS_ResourceSet_H_
#define	_NZP_CSI_RS_ResourceSet_H_


#include <asn_application.h>

/* Including external dependencies */
#include "NZP-CSI-RS-ResourceSetId.h"
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include "NZP-CSI-RS-ResourceId.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum NZP_CSI_RS_ResourceSet__repetition {
	NZP_CSI_RS_ResourceSet__repetition_on	= 0,
	NZP_CSI_RS_ResourceSet__repetition_off	= 1
} e_NZP_CSI_RS_ResourceSet__repetition;
typedef enum NZP_CSI_RS_ResourceSet__trs_Info {
	NZP_CSI_RS_ResourceSet__trs_Info_true	= 0
} e_NZP_CSI_RS_ResourceSet__trs_Info;

/* NZP-CSI-RS-ResourceSet */
typedef struct NZP_CSI_RS_ResourceSet {
	NZP_CSI_RS_ResourceSetId_t	 nzp_CSI_ResourceSetId;
	struct NZP_CSI_RS_ResourceSet__nzp_CSI_RS_Resources {
		A_SEQUENCE_OF(NZP_CSI_RS_ResourceId_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} nzp_CSI_RS_Resources;
	long	*repetition;	/* OPTIONAL */
	long	*aperiodicTriggeringOffset;	/* OPTIONAL */
	long	*trs_Info;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} NZP_CSI_RS_ResourceSet_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_repetition_5;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_trs_Info_9;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_NZP_CSI_RS_ResourceSet;
extern asn_SEQUENCE_specifics_t asn_SPC_NZP_CSI_RS_ResourceSet_specs_1;
extern asn_TYPE_member_t asn_MBR_NZP_CSI_RS_ResourceSet_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _NZP_CSI_RS_ResourceSet_H_ */
#include <asn_internal.h>
