/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PDSCH_TimeDomainResourceAllocation_H_
#define	_PDSCH_TimeDomainResourceAllocation_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PDSCH_TimeDomainResourceAllocation__mappingType {
	PDSCH_TimeDomainResourceAllocation__mappingType_typeA	= 0,
	PDSCH_TimeDomainResourceAllocation__mappingType_typeB	= 1
} e_PDSCH_TimeDomainResourceAllocation__mappingType;

/* PDSCH-TimeDomainResourceAllocation */
typedef struct PDSCH_TimeDomainResourceAllocation {
	long	*k0;	/* OPTIONAL */
	long	 mappingType;
	long	 startSymbolAndLength;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PDSCH_TimeDomainResourceAllocation_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_mappingType_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PDSCH_TimeDomainResourceAllocation;
extern asn_SEQUENCE_specifics_t asn_SPC_PDSCH_TimeDomainResourceAllocation_specs_1;
extern asn_TYPE_member_t asn_MBR_PDSCH_TimeDomainResourceAllocation_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _PDSCH_TimeDomainResourceAllocation_H_ */
#include <asn_internal.h>
