/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_EUTRA_RSTD_Info_H_
#define	_EUTRA_RSTD_Info_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ARFCN-ValueEUTRA.h"
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EUTRA-RSTD-Info */
typedef struct EUTRA_RSTD_Info {
	ARFCN_ValueEUTRA_t	 carrierFreq;
	long	 measPRS_Offset;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EUTRA_RSTD_Info_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EUTRA_RSTD_Info;
extern asn_SEQUENCE_specifics_t asn_SPC_EUTRA_RSTD_Info_specs_1;
extern asn_TYPE_member_t asn_MBR_EUTRA_RSTD_Info_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _EUTRA_RSTD_Info_H_ */
#include <asn_internal.h>
