/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SIB_TypeInfo_H_
#define	_SIB_TypeInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SIB_TypeInfo__type {
	SIB_TypeInfo__type_sibType2	= 0,
	SIB_TypeInfo__type_sibType3	= 1,
	SIB_TypeInfo__type_sibType4	= 2,
	SIB_TypeInfo__type_sibType5	= 3,
	SIB_TypeInfo__type_sibType6	= 4,
	SIB_TypeInfo__type_sibType7	= 5,
	SIB_TypeInfo__type_sibType8	= 6,
	SIB_TypeInfo__type_sibType9	= 7,
	SIB_TypeInfo__type_spare8	= 8,
	SIB_TypeInfo__type_spare7	= 9,
	SIB_TypeInfo__type_spare6	= 10,
	SIB_TypeInfo__type_spare5	= 11,
	SIB_TypeInfo__type_spare4	= 12,
	SIB_TypeInfo__type_spare3	= 13,
	SIB_TypeInfo__type_spare2	= 14,
	SIB_TypeInfo__type_spare1	= 15
	/*
	 * Enumeration is extensible
	 */
} e_SIB_TypeInfo__type;
typedef enum SIB_TypeInfo__areaScope {
	SIB_TypeInfo__areaScope_true	= 0
} e_SIB_TypeInfo__areaScope;

/* SIB-TypeInfo */
typedef struct SIB_TypeInfo {
	long	 type;
	long	*valueTag;	/* OPTIONAL */
	long	*areaScope;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SIB_TypeInfo_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_type_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_areaScope_21;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_SIB_TypeInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_SIB_TypeInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_SIB_TypeInfo_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _SIB_TypeInfo_H_ */
#include <asn_internal.h>
