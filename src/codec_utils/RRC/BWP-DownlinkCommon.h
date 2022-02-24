/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_BWP_DownlinkCommon_H_
#define	_BWP_DownlinkCommon_H_


#include <asn_application.h>

/* Including external dependencies */
#include "BWP.h"
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BWP_DownlinkCommon__pdcch_ConfigCommon_PR {
	BWP_DownlinkCommon__pdcch_ConfigCommon_PR_NOTHING,	/* No components present */
	BWP_DownlinkCommon__pdcch_ConfigCommon_PR_release,
	BWP_DownlinkCommon__pdcch_ConfigCommon_PR_setup
} BWP_DownlinkCommon__pdcch_ConfigCommon_PR;
typedef enum BWP_DownlinkCommon__pdsch_ConfigCommon_PR {
	BWP_DownlinkCommon__pdsch_ConfigCommon_PR_NOTHING,	/* No components present */
	BWP_DownlinkCommon__pdsch_ConfigCommon_PR_release,
	BWP_DownlinkCommon__pdsch_ConfigCommon_PR_setup
} BWP_DownlinkCommon__pdsch_ConfigCommon_PR;

/* Forward declarations */
struct PDCCH_ConfigCommon;
struct PDSCH_ConfigCommon;

/* BWP-DownlinkCommon */
typedef struct BWP_DownlinkCommon {
	BWP_t	 genericParameters;
	struct BWP_DownlinkCommon__pdcch_ConfigCommon {
		BWP_DownlinkCommon__pdcch_ConfigCommon_PR present;
		union BWP_DownlinkCommon__pdcch_ConfigCommon_u {
			NULL_t	 release;
			struct PDCCH_ConfigCommon	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *pdcch_ConfigCommon;
	struct BWP_DownlinkCommon__pdsch_ConfigCommon {
		BWP_DownlinkCommon__pdsch_ConfigCommon_PR present;
		union BWP_DownlinkCommon__pdsch_ConfigCommon_u {
			NULL_t	 release;
			struct PDSCH_ConfigCommon	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *pdsch_ConfigCommon;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BWP_DownlinkCommon_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BWP_DownlinkCommon;
extern asn_SEQUENCE_specifics_t asn_SPC_BWP_DownlinkCommon_specs_1;
extern asn_TYPE_member_t asn_MBR_BWP_DownlinkCommon_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _BWP_DownlinkCommon_H_ */
#include <asn_internal.h>
