/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "codeathon/E2SM_KPM.asn1"
 * 	`asn1c -D ./codeathon/e2smkpm/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_OCUUP_PF_Container_H_
#define	_OCUUP_PF_Container_H_


#include <asn_application.h>

/* Including external dependencies */
#include "GNB-CU-UP-Name.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct PF_ContainerListItem;

/* OCUUP-PF-Container */
typedef struct OCUUP_PF_Container {
	GNB_CU_UP_Name_t	*gNB_CU_UP_Name;	/* OPTIONAL */
	struct OCUUP_PF_Container__pf_ContainerList {
		A_SEQUENCE_OF(struct PF_ContainerListItem) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} pf_ContainerList;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} OCUUP_PF_Container_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_OCUUP_PF_Container;
extern asn_SEQUENCE_specifics_t asn_SPC_OCUUP_PF_Container_specs_1;
extern asn_TYPE_member_t asn_MBR_OCUUP_PF_Container_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _OCUUP_PF_Container_H_ */
#include <asn_internal.h>
