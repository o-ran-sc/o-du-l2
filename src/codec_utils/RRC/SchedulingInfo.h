/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SchedulingInfo_H_
#define	_SchedulingInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include "SIB-Mapping.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SchedulingInfo__si_BroadcastStatus {
	SchedulingInfo__si_BroadcastStatus_broadcasting	= 0,
	SchedulingInfo__si_BroadcastStatus_notBroadcasting	= 1
} e_SchedulingInfo__si_BroadcastStatus;
typedef enum SchedulingInfo__si_Periodicity {
	SchedulingInfo__si_Periodicity_rf8	= 0,
	SchedulingInfo__si_Periodicity_rf16	= 1,
	SchedulingInfo__si_Periodicity_rf32	= 2,
	SchedulingInfo__si_Periodicity_rf64	= 3,
	SchedulingInfo__si_Periodicity_rf128	= 4,
	SchedulingInfo__si_Periodicity_rf256	= 5,
	SchedulingInfo__si_Periodicity_rf512	= 6
} e_SchedulingInfo__si_Periodicity;

/* SchedulingInfo */
typedef struct SchedulingInfo {
	long	 si_BroadcastStatus;
	long	 si_Periodicity;
	SIB_Mapping_t	 sib_MappingInfo;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SchedulingInfo_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_si_BroadcastStatus_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_si_Periodicity_5;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_SchedulingInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_SchedulingInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_SchedulingInfo_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _SchedulingInfo_H_ */
#include <asn_internal.h>
