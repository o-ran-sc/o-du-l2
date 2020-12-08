/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasAndMobParametersXDD_Diff_H_
#define	_MeasAndMobParametersXDD_Diff_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MeasAndMobParametersXDD_Diff__intraAndInterF_MeasAndReport {
	MeasAndMobParametersXDD_Diff__intraAndInterF_MeasAndReport_supported	= 0
} e_MeasAndMobParametersXDD_Diff__intraAndInterF_MeasAndReport;
typedef enum MeasAndMobParametersXDD_Diff__eventA_MeasAndReport {
	MeasAndMobParametersXDD_Diff__eventA_MeasAndReport_supported	= 0
} e_MeasAndMobParametersXDD_Diff__eventA_MeasAndReport;
typedef enum MeasAndMobParametersXDD_Diff__ext1__handoverInterF {
	MeasAndMobParametersXDD_Diff__ext1__handoverInterF_supported	= 0
} e_MeasAndMobParametersXDD_Diff__ext1__handoverInterF;
typedef enum MeasAndMobParametersXDD_Diff__ext1__handoverLTE {
	MeasAndMobParametersXDD_Diff__ext1__handoverLTE_supported	= 0
} e_MeasAndMobParametersXDD_Diff__ext1__handoverLTE;
typedef enum MeasAndMobParametersXDD_Diff__ext1__handover_eLTE {
	MeasAndMobParametersXDD_Diff__ext1__handover_eLTE_supported	= 0
} e_MeasAndMobParametersXDD_Diff__ext1__handover_eLTE;

/* MeasAndMobParametersXDD-Diff */
typedef struct MeasAndMobParametersXDD_Diff {
	long	*intraAndInterF_MeasAndReport;	/* OPTIONAL */
	long	*eventA_MeasAndReport;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct MeasAndMobParametersXDD_Diff__ext1 {
		long	*handoverInterF;	/* OPTIONAL */
		long	*handoverLTE;	/* OPTIONAL */
		long	*handover_eLTE;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasAndMobParametersXDD_Diff_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_intraAndInterF_MeasAndReport_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_eventA_MeasAndReport_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_handoverInterF_8;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_handoverLTE_10;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_handover_eLTE_12;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_MeasAndMobParametersXDD_Diff;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasAndMobParametersXDD_Diff_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasAndMobParametersXDD_Diff_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _MeasAndMobParametersXDD_Diff_H_ */
#include <asn_internal.h>
