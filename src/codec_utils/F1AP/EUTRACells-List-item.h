/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_EUTRACells_List_item_H_
#define	_EUTRACells_List_item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "EUTRA-Cell-ID.h"
#include "Served-EUTRA-Cells-Information.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolExtensionContainer;

/* EUTRACells-List-item */
typedef struct EUTRACells_List_item {
	EUTRA_Cell_ID_t	 eUTRA_Cell_ID;
	Served_EUTRA_Cells_Information_t	 served_EUTRA_Cells_Information;
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EUTRACells_List_item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EUTRACells_List_item;
extern asn_SEQUENCE_specifics_t asn_SPC_EUTRACells_List_item_specs_1;
extern asn_TYPE_member_t asn_MBR_EUTRACells_List_item_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _EUTRACells_List_item_H_ */
#include <asn_internal.h>
