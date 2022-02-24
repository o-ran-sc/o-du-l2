/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PDCCH_ServingCellConfig_H_
#define	_PDCCH_ServingCellConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PDCCH_ServingCellConfig__slotFormatIndicator_PR {
	PDCCH_ServingCellConfig__slotFormatIndicator_PR_NOTHING,	/* No components present */
	PDCCH_ServingCellConfig__slotFormatIndicator_PR_release,
	PDCCH_ServingCellConfig__slotFormatIndicator_PR_setup
} PDCCH_ServingCellConfig__slotFormatIndicator_PR;

/* Forward declarations */
struct SlotFormatIndicator;

/* PDCCH-ServingCellConfig */
typedef struct PDCCH_ServingCellConfig {
	struct PDCCH_ServingCellConfig__slotFormatIndicator {
		PDCCH_ServingCellConfig__slotFormatIndicator_PR present;
		union PDCCH_ServingCellConfig__slotFormatIndicator_u {
			NULL_t	 release;
			struct SlotFormatIndicator	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *slotFormatIndicator;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PDCCH_ServingCellConfig_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PDCCH_ServingCellConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_PDCCH_ServingCellConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_PDCCH_ServingCellConfig_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _PDCCH_ServingCellConfig_H_ */
#include <asn_internal.h>
