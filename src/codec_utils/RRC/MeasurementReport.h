/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_MeasurementReport_H_
#define	_MeasurementReport_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MeasurementReport__criticalExtensions_PR {
	MeasurementReport__criticalExtensions_PR_NOTHING,	/* No components present */
	MeasurementReport__criticalExtensions_PR_measurementReport,
	MeasurementReport__criticalExtensions_PR_criticalExtensionsFuture
} MeasurementReport__criticalExtensions_PR;

/* Forward declarations */
struct MeasurementReport_IEs;

/* MeasurementReport */
typedef struct MeasurementReport {
	struct MeasurementReport__criticalExtensions {
		MeasurementReport__criticalExtensions_PR present;
		union MeasurementReport__criticalExtensions_u {
			struct MeasurementReport_IEs	*measurementReport;
			struct MeasurementReport__criticalExtensions__criticalExtensionsFuture {
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *criticalExtensionsFuture;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} criticalExtensions;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasurementReport_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasurementReport;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasurementReport_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasurementReport_1[1];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "MeasurementReport-IEs.h"

#endif	/* _MeasurementReport_H_ */
#include <asn_internal.h>
