/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_PUCCH_PathlossReferenceRS_H_
#define	_PUCCH_PathlossReferenceRS_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PUCCH-PathlossReferenceRS-Id.h"
#include "SSB-Index.h"
#include "NZP-CSI-RS-ResourceId.h"
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PUCCH_PathlossReferenceRS__referenceSignal_PR {
	PUCCH_PathlossReferenceRS__referenceSignal_PR_NOTHING,	/* No components present */
	PUCCH_PathlossReferenceRS__referenceSignal_PR_ssb_Index,
	PUCCH_PathlossReferenceRS__referenceSignal_PR_csi_RS_Index
} PUCCH_PathlossReferenceRS__referenceSignal_PR;

/* PUCCH-PathlossReferenceRS */
typedef struct PUCCH_PathlossReferenceRS {
	PUCCH_PathlossReferenceRS_Id_t	 pucch_PathlossReferenceRS_Id;
	struct PUCCH_PathlossReferenceRS__referenceSignal {
		PUCCH_PathlossReferenceRS__referenceSignal_PR present;
		union PUCCH_PathlossReferenceRS__referenceSignal_u {
			SSB_Index_t	 ssb_Index;
			NZP_CSI_RS_ResourceId_t	 csi_RS_Index;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} referenceSignal;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PUCCH_PathlossReferenceRS_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PUCCH_PathlossReferenceRS;
extern asn_SEQUENCE_specifics_t asn_SPC_PUCCH_PathlossReferenceRS_specs_1;
extern asn_TYPE_member_t asn_MBR_PUCCH_PathlossReferenceRS_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _PUCCH_PathlossReferenceRS_H_ */
#include <asn_internal.h>
