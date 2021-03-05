/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "codeathon/E2SM_KPM.asn1"
 * 	`asn1c -D ./codeathon/e2smkpm/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_FQIPERSlicesPerPlmnListItem_H_
#define	_FQIPERSlicesPerPlmnListItem_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <INTEGER.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* FQIPERSlicesPerPlmnListItem */
typedef struct FQIPERSlicesPerPlmnListItem {
	long	 fiveQI;
	INTEGER_t	*pDCPBytesDL;	/* OPTIONAL */
	INTEGER_t	*pDCPBytesUL;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FQIPERSlicesPerPlmnListItem_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FQIPERSlicesPerPlmnListItem;
extern asn_SEQUENCE_specifics_t asn_SPC_FQIPERSlicesPerPlmnListItem_specs_1;
extern asn_TYPE_member_t asn_MBR_FQIPERSlicesPerPlmnListItem_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _FQIPERSlicesPerPlmnListItem_H_ */
#include <asn_internal.h>
