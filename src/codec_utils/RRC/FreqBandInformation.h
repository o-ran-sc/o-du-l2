/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_FreqBandInformation_H_
#define	_FreqBandInformation_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum FreqBandInformation_PR {
	FreqBandInformation_PR_NOTHING,	/* No components present */
	FreqBandInformation_PR_bandInformationEUTRA,
	FreqBandInformation_PR_bandInformationNR
} FreqBandInformation_PR;

/* Forward declarations */
struct FreqBandInformationEUTRA;
struct FreqBandInformationNR;

/* FreqBandInformation */
typedef struct FreqBandInformation {
	FreqBandInformation_PR present;
	union FreqBandInformation_u {
		struct FreqBandInformationEUTRA	*bandInformationEUTRA;
		struct FreqBandInformationNR	*bandInformationNR;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FreqBandInformation_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FreqBandInformation;
extern asn_CHOICE_specifics_t asn_SPC_FreqBandInformation_specs_1;
extern asn_TYPE_member_t asn_MBR_FreqBandInformation_1[2];
extern asn_per_constraints_t asn_PER_type_FreqBandInformation_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _FreqBandInformation_H_ */
#include <asn_internal.h>
