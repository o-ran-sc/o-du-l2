/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_Cells_to_be_Activated_List_Item_H_
#define	_Cells_to_be_Activated_List_Item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "NRCGI.h"
#include "NRPCI.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolExtensionContainer;

/* Cells-to-be-Activated-List-Item */
typedef struct Cells_to_be_Activated_List_Item {
	NRCGI_t	 nRCGI;
	NRPCI_t	*nRPCI;	/* OPTIONAL */
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Cells_to_be_Activated_List_Item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Cells_to_be_Activated_List_Item;
extern asn_SEQUENCE_specifics_t asn_SPC_Cells_to_be_Activated_List_Item_specs_1;
extern asn_TYPE_member_t asn_MBR_Cells_to_be_Activated_List_Item_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _Cells_to_be_Activated_List_Item_H_ */
#include <asn_internal.h>
