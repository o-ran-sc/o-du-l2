/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "codeathon/E2SM_KPM.asn1"
 * 	`asn1c -D ./codeathon/e2smkpm/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_GlobalKPMnode_en_gNB_ID_H_
#define	_GlobalKPMnode_en_gNB_ID_H_


#include <asn_application.h>

/* Including external dependencies */
#include "GlobalenGNB-ID.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* GlobalKPMnode-en-gNB-ID */
typedef struct GlobalKPMnode_en_gNB_ID {
	GlobalenGNB_ID_t	 global_gNB_ID;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} GlobalKPMnode_en_gNB_ID_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_GlobalKPMnode_en_gNB_ID;
extern asn_SEQUENCE_specifics_t asn_SPC_GlobalKPMnode_en_gNB_ID_specs_1;
extern asn_TYPE_member_t asn_MBR_GlobalKPMnode_en_gNB_ID_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _GlobalKPMnode_en_gNB_ID_H_ */
#include <asn_internal.h>
