/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "codeathon/E2AP.asn1"
 * 	`asn1c -D ./newE2AP -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RICaction_NotAdmitted_Item_H_
#define	_RICaction_NotAdmitted_Item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RICactionID.h"
#include "CauseE2.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RICaction-NotAdmitted-Item */
typedef struct RICaction_NotAdmitted_Item {
	RICactionID_t	 ricActionID;
	CauseE2_t	 cause;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RICaction_NotAdmitted_Item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RICaction_NotAdmitted_Item;

#ifdef __cplusplus
}
#endif

#endif	/* _RICaction_NotAdmitted_Item_H_ */
#include <asn_internal.h>
