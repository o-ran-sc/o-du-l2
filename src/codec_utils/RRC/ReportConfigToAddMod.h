/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_ReportConfigToAddMod_H_
#define	_ReportConfigToAddMod_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ReportConfigId.h"
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ReportConfigToAddMod__reportConfig_PR {
	ReportConfigToAddMod__reportConfig_PR_NOTHING,	/* No components present */
	ReportConfigToAddMod__reportConfig_PR_reportConfigNR,
	/* Extensions may appear below */
	ReportConfigToAddMod__reportConfig_PR_reportConfigInterRAT
} ReportConfigToAddMod__reportConfig_PR;

/* Forward declarations */
struct ReportConfigNR;
struct ReportConfigInterRAT;

/* ReportConfigToAddMod */
typedef struct ReportConfigToAddMod {
	ReportConfigId_t	 reportConfigId;
	struct ReportConfigToAddMod__reportConfig {
		ReportConfigToAddMod__reportConfig_PR present;
		union ReportConfigToAddMod__reportConfig_u {
			struct ReportConfigNR	*reportConfigNR;
			/*
			 * This type is extensible,
			 * possible extensions are below.
			 */
			struct ReportConfigInterRAT	*reportConfigInterRAT;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} reportConfig;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ReportConfigToAddMod_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ReportConfigToAddMod;
extern asn_SEQUENCE_specifics_t asn_SPC_ReportConfigToAddMod_specs_1;
extern asn_TYPE_member_t asn_MBR_ReportConfigToAddMod_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _ReportConfigToAddMod_H_ */
#include <asn_internal.h>
