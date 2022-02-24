/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_ReportConfigInterRAT_H_
#define	_ReportConfigInterRAT_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ReportConfigInterRAT__reportType_PR {
	ReportConfigInterRAT__reportType_PR_NOTHING,	/* No components present */
	ReportConfigInterRAT__reportType_PR_periodical,
	ReportConfigInterRAT__reportType_PR_eventTriggered,
	ReportConfigInterRAT__reportType_PR_reportCGI
	/* Extensions may appear below */
	
} ReportConfigInterRAT__reportType_PR;

/* Forward declarations */
struct PeriodicalReportConfigInterRAT;
struct EventTriggerConfigInterRAT;
struct ReportCGI_EUTRA;

/* ReportConfigInterRAT */
typedef struct ReportConfigInterRAT {
	struct ReportConfigInterRAT__reportType {
		ReportConfigInterRAT__reportType_PR present;
		union ReportConfigInterRAT__reportType_u {
			struct PeriodicalReportConfigInterRAT	*periodical;
			struct EventTriggerConfigInterRAT	*eventTriggered;
			struct ReportCGI_EUTRA	*reportCGI;
			/*
			 * This type is extensible,
			 * possible extensions are below.
			 */
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} reportType;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ReportConfigInterRAT_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ReportConfigInterRAT;
extern asn_SEQUENCE_specifics_t asn_SPC_ReportConfigInterRAT_specs_1;
extern asn_TYPE_member_t asn_MBR_ReportConfigInterRAT_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _ReportConfigInterRAT_H_ */
#include <asn_internal.h>
