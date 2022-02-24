/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_BandParameters_H_
#define	_BandParameters_H_


#include <asn_application.h>

/* Including external dependencies */
#include "FreqBandIndicatorEUTRA.h"
#include "CA-BandwidthClassEUTRA.h"
#include <constr_SEQUENCE.h>
#include "FreqBandIndicatorNR.h"
#include "CA-BandwidthClassNR.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BandParameters_PR {
	BandParameters_PR_NOTHING,	/* No components present */
	BandParameters_PR_eutra,
	BandParameters_PR_nr
} BandParameters_PR;

/* BandParameters */
typedef struct BandParameters {
	BandParameters_PR present;
	union BandParameters_u {
		struct BandParameters__eutra {
			FreqBandIndicatorEUTRA_t	 bandEUTRA;
			CA_BandwidthClassEUTRA_t	*ca_BandwidthClassDL_EUTRA;	/* OPTIONAL */
			CA_BandwidthClassEUTRA_t	*ca_BandwidthClassUL_EUTRA;	/* OPTIONAL */
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *eutra;
		struct BandParameters__nr {
			FreqBandIndicatorNR_t	 bandNR;
			CA_BandwidthClassNR_t	*ca_BandwidthClassDL_NR;	/* OPTIONAL */
			CA_BandwidthClassNR_t	*ca_BandwidthClassUL_NR;	/* OPTIONAL */
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *nr;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BandParameters_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BandParameters;
extern asn_CHOICE_specifics_t asn_SPC_BandParameters_specs_1;
extern asn_TYPE_member_t asn_MBR_BandParameters_1[2];
extern asn_per_constraints_t asn_PER_type_BandParameters_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _BandParameters_H_ */
#include <asn_internal.h>
