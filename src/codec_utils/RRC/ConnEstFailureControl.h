/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_ConnEstFailureControl_H_
#define	_ConnEstFailureControl_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ConnEstFailureControl__connEstFailCount {
	ConnEstFailureControl__connEstFailCount_n1	= 0,
	ConnEstFailureControl__connEstFailCount_n2	= 1,
	ConnEstFailureControl__connEstFailCount_n3	= 2,
	ConnEstFailureControl__connEstFailCount_n4	= 3
} e_ConnEstFailureControl__connEstFailCount;
typedef enum ConnEstFailureControl__connEstFailOffsetValidity {
	ConnEstFailureControl__connEstFailOffsetValidity_s30	= 0,
	ConnEstFailureControl__connEstFailOffsetValidity_s60	= 1,
	ConnEstFailureControl__connEstFailOffsetValidity_s120	= 2,
	ConnEstFailureControl__connEstFailOffsetValidity_s240	= 3,
	ConnEstFailureControl__connEstFailOffsetValidity_s300	= 4,
	ConnEstFailureControl__connEstFailOffsetValidity_s420	= 5,
	ConnEstFailureControl__connEstFailOffsetValidity_s600	= 6,
	ConnEstFailureControl__connEstFailOffsetValidity_s900	= 7
} e_ConnEstFailureControl__connEstFailOffsetValidity;

/* ConnEstFailureControl */
typedef struct ConnEstFailureControl {
	long	 connEstFailCount;
	long	 connEstFailOffsetValidity;
	long	*connEstFailOffset;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ConnEstFailureControl_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_connEstFailCount_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_connEstFailOffsetValidity_7;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_ConnEstFailureControl;
extern asn_SEQUENCE_specifics_t asn_SPC_ConnEstFailureControl_specs_1;
extern asn_TYPE_member_t asn_MBR_ConnEstFailureControl_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _ConnEstFailureControl_H_ */
#include <asn_internal.h>
