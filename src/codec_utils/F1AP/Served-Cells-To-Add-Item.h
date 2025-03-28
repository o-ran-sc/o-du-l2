/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_Served_Cells_To_Add_Item_H_
#define	_Served_Cells_To_Add_Item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Served-Cell-Information.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct GNB_DU_System_Information;
struct ProtocolExtensionContainer;

/* Served-Cells-To-Add-Item */
typedef struct Served_Cells_To_Add_Item {
	Served_Cell_Information_t	 served_Cell_Information;
	struct GNB_DU_System_Information	*gNB_DU_System_Information;	/* OPTIONAL */
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Served_Cells_To_Add_Item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Served_Cells_To_Add_Item;
extern asn_SEQUENCE_specifics_t asn_SPC_Served_Cells_To_Add_Item_specs_1;
extern asn_TYPE_member_t asn_MBR_Served_Cells_To_Add_Item_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _Served_Cells_To_Add_Item_H_ */
#include <asn_internal.h>
