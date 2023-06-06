/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_E2nodeComponentConfigAddition_Item_H_
#define	_E2nodeComponentConfigAddition_Item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "E2nodeComponentInterfaceType.h"
#include "E2nodeComponentID.h"
#include "E2nodeComponentConfiguration.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* E2nodeComponentConfigAddition-Item */
typedef struct E2nodeComponentConfigAddition_Item {
	E2nodeComponentInterfaceType_t	 e2nodeComponentInterfaceType;
	E2nodeComponentID_t	 e2nodeComponentID;
	E2nodeComponentConfiguration_t	 e2nodeComponentConfiguration;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} E2nodeComponentConfigAddition_Item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_E2nodeComponentConfigAddition_Item;

#ifdef __cplusplus
}
#endif

#endif	/* _E2nodeComponentConfigAddition_Item_H_ */
#include <asn_internal.h>
