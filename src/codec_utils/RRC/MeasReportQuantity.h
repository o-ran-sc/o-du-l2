/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasReportQuantity_H_
#define	_MeasReportQuantity_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BOOLEAN.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* MeasReportQuantity */
typedef struct MeasReportQuantity {
	BOOLEAN_t	 rsrp;
	BOOLEAN_t	 rsrq;
	BOOLEAN_t	 sinr;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasReportQuantity_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasReportQuantity;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasReportQuantity_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasReportQuantity_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _MeasReportQuantity_H_ */
#include <asn_internal.h>
