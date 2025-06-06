/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/E2APV0300.asn1"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2AP/`
 */

#ifndef	_ENB_ID_Choice_H_
#define	_ENB_ID_Choice_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ENB_ID_Choice_PR {
	ENB_ID_Choice_PR_NOTHING,	/* No components present */
	ENB_ID_Choice_PR_enb_ID_macro,
	ENB_ID_Choice_PR_enb_ID_shortmacro,
	ENB_ID_Choice_PR_enb_ID_longmacro
	/* Extensions may appear below */
	
} ENB_ID_Choice_PR;

/* ENB-ID-Choice */
typedef struct ENB_ID_Choice {
	ENB_ID_Choice_PR present;
	union ENB_ID_Choice_u {
		BIT_STRING_t	 enb_ID_macro;
		BIT_STRING_t	 enb_ID_shortmacro;
		BIT_STRING_t	 enb_ID_longmacro;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ENB_ID_Choice_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ENB_ID_Choice;
extern asn_CHOICE_specifics_t asn_SPC_ENB_ID_Choice_specs_1;
extern asn_TYPE_member_t asn_MBR_ENB_ID_Choice_1[3];
extern asn_per_constraints_t asn_PER_type_ENB_ID_Choice_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _ENB_ID_Choice_H_ */
#include <asn_internal.h>
