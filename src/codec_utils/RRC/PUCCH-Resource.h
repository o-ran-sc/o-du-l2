/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PUCCH_Resource_H_
#define	_PUCCH_Resource_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PUCCH-ResourceId.h"
#include "PRB-Id.h"
#include <NativeEnumerated.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PUCCH_Resource__intraSlotFrequencyHopping {
	PUCCH_Resource__intraSlotFrequencyHopping_enabled	= 0
} e_PUCCH_Resource__intraSlotFrequencyHopping;
typedef enum PUCCH_Resource__format_PR {
	PUCCH_Resource__format_PR_NOTHING,	/* No components present */
	PUCCH_Resource__format_PR_format0,
	PUCCH_Resource__format_PR_format1,
	PUCCH_Resource__format_PR_format2,
	PUCCH_Resource__format_PR_format3,
	PUCCH_Resource__format_PR_format4
} PUCCH_Resource__format_PR;

/* Forward declarations */
struct PUCCH_format0;
struct PUCCH_format1;
struct PUCCH_format2;
struct PUCCH_format3;
struct PUCCH_format4;

/* PUCCH-Resource */
typedef struct PUCCH_Resource {
	PUCCH_ResourceId_t	 pucch_ResourceId;
	PRB_Id_t	 startingPRB;
	long	*intraSlotFrequencyHopping;	/* OPTIONAL */
	PRB_Id_t	*secondHopPRB;	/* OPTIONAL */
	struct PUCCH_Resource__format {
		PUCCH_Resource__format_PR present;
		union PUCCH_Resource__format_u {
			struct PUCCH_format0	*format0;
			struct PUCCH_format1	*format1;
			struct PUCCH_format2	*format2;
			struct PUCCH_format3	*format3;
			struct PUCCH_format4	*format4;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} format;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PUCCH_Resource_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_intraSlotFrequencyHopping_4;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PUCCH_Resource;
extern asn_SEQUENCE_specifics_t asn_SPC_PUCCH_Resource_specs_1;
extern asn_TYPE_member_t asn_MBR_PUCCH_Resource_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _PUCCH_Resource_H_ */
#include <asn_internal.h>
