/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_KPM_v2.03.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#ifndef	_E2SM_KPM_IndicationHeader_H_
#define	_E2SM_KPM_IndicationHeader_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum E2SM_KPM_IndicationHeader__indicationHeader_formats_PR {
	E2SM_KPM_IndicationHeader__indicationHeader_formats_PR_NOTHING,	/* No components present */
	E2SM_KPM_IndicationHeader__indicationHeader_formats_PR_indicationHeader_Format1
	/* Extensions may appear below */
	
} E2SM_KPM_IndicationHeader__indicationHeader_formats_PR;

/* Forward declarations */
struct E2SM_KPM_IndicationHeader_Format1;

/* E2SM-KPM-IndicationHeader */
typedef struct E2SM_KPM_IndicationHeader {
	struct E2SM_KPM_IndicationHeader__indicationHeader_formats {
		E2SM_KPM_IndicationHeader__indicationHeader_formats_PR present;
		union E2SM_KPM_IndicationHeader__indicationHeader_formats_u {
			struct E2SM_KPM_IndicationHeader_Format1	*indicationHeader_Format1;
			/*
			 * This type is extensible,
			 * possible extensions are below.
			 */
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} indicationHeader_formats;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} E2SM_KPM_IndicationHeader_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_E2SM_KPM_IndicationHeader;

#ifdef __cplusplus
}
#endif

#endif	/* _E2SM_KPM_IndicationHeader_H_ */
#include <asn_internal.h>
