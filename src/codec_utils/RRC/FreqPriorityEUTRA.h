/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_FreqPriorityEUTRA_H_
#define	_FreqPriorityEUTRA_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ARFCN-ValueEUTRA.h"
#include "CellReselectionPriority.h"
#include "CellReselectionSubPriority.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* FreqPriorityEUTRA */
typedef struct FreqPriorityEUTRA {
	ARFCN_ValueEUTRA_t	 carrierFreq;
	CellReselectionPriority_t	 cellReselectionPriority;
	CellReselectionSubPriority_t	*cellReselectionSubPriority;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FreqPriorityEUTRA_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FreqPriorityEUTRA;
extern asn_SEQUENCE_specifics_t asn_SPC_FreqPriorityEUTRA_specs_1;
extern asn_TYPE_member_t asn_MBR_FreqPriorityEUTRA_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _FreqPriorityEUTRA_H_ */
#include <asn_internal.h>
