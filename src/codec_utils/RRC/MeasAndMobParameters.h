/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasAndMobParameters_H_
#define	_MeasAndMobParameters_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct MeasAndMobParametersCommon;
struct MeasAndMobParametersXDD_Diff;
struct MeasAndMobParametersFRX_Diff;

/* MeasAndMobParameters */
typedef struct MeasAndMobParameters {
	struct MeasAndMobParametersCommon	*measAndMobParametersCommon;	/* OPTIONAL */
	struct MeasAndMobParametersXDD_Diff	*measAndMobParametersXDD_Diff;	/* OPTIONAL */
	struct MeasAndMobParametersFRX_Diff	*measAndMobParametersFRX_Diff;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasAndMobParameters_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasAndMobParameters;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasAndMobParameters_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasAndMobParameters_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _MeasAndMobParameters_H_ */
#include <asn_internal.h>
