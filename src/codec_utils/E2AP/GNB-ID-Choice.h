/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "E2AP.asn1"
 * 	`asn1c -D ./out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_GNB_ID_Choice_H_
#define	_GNB_ID_Choice_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum GNB_ID_Choice_PR {
	GNB_ID_Choice_PR_NOTHING,	/* No components present */
	GNB_ID_Choice_PR_gnb_ID
	/* Extensions may appear below */
	
} GNB_ID_Choice_PR;

/* GNB-ID-Choice */
typedef struct GNB_ID_Choice {
	GNB_ID_Choice_PR present;
	union GNB_ID_Choice_u {
		BIT_STRING_t	 gnb_ID;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} GNB_ID_Choice_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_GNB_ID_Choice;
extern asn_CHOICE_specifics_t asn_SPC_GNB_ID_Choice_specs_1;
extern asn_TYPE_member_t asn_MBR_GNB_ID_Choice_1[1];
extern asn_per_constraints_t asn_PER_type_GNB_ID_Choice_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _GNB_ID_Choice_H_ */
#include <asn_internal.h>
