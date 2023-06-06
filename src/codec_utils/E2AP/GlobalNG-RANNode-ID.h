/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_GlobalNG_RANNode_ID_H_
#define	_GlobalNG_RANNode_ID_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum GlobalNG_RANNode_ID_PR {
	GlobalNG_RANNode_ID_PR_NOTHING,	/* No components present */
	GlobalNG_RANNode_ID_PR_gNB,
	GlobalNG_RANNode_ID_PR_ng_eNB
	/* Extensions may appear below */
	
} GlobalNG_RANNode_ID_PR;

/* Forward declarations */
struct GlobalgNB_ID;
struct GlobalngeNB_ID;

/* GlobalNG-RANNode-ID */
typedef struct GlobalNG_RANNode_ID {
	GlobalNG_RANNode_ID_PR present;
	union GlobalNG_RANNode_ID_u {
		struct GlobalgNB_ID	*gNB;
		struct GlobalngeNB_ID	*ng_eNB;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} GlobalNG_RANNode_ID_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_GlobalNG_RANNode_ID;
extern asn_CHOICE_specifics_t asn_SPC_GlobalNG_RANNode_ID_specs_1;
extern asn_TYPE_member_t asn_MBR_GlobalNG_RANNode_ID_1[2];
extern asn_per_constraints_t asn_PER_type_GlobalNG_RANNode_ID_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _GlobalNG_RANNode_ID_H_ */
#include <asn_internal.h>
