/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "2022_E2AP.asn1"
 * 	`asn1c -D ./E2AP/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RANfunctionID_Item_H_
#define	_RANfunctionID_Item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RANfunctionID.h"
#include "RANfunctionRevision.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RANfunctionID-Item */
typedef struct RANfunctionID_Item {
	RANfunctionID_t	 ranFunctionID;
	RANfunctionRevision_t	 ranFunctionRevision;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RANfunctionID_Item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RANfunctionID_Item;

#ifdef __cplusplus
}
#endif

#endif	/* _RANfunctionID_Item_H_ */
#include <asn_internal.h>
