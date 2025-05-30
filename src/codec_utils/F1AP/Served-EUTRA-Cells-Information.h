/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_Served_EUTRA_Cells_Information_H_
#define	_Served_EUTRA_Cells_Information_H_


#include <asn_application.h>

/* Including external dependencies */
#include "EUTRA-Mode-Info.h"
#include "ProtectedEUTRAResourceIndication.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolExtensionContainer;

/* Served-EUTRA-Cells-Information */
typedef struct Served_EUTRA_Cells_Information {
	EUTRA_Mode_Info_t	 eUTRA_Mode_Info;
	ProtectedEUTRAResourceIndication_t	 protectedEUTRAResourceIndication;
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Served_EUTRA_Cells_Information_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Served_EUTRA_Cells_Information;
extern asn_SEQUENCE_specifics_t asn_SPC_Served_EUTRA_Cells_Information_specs_1;
extern asn_TYPE_member_t asn_MBR_Served_EUTRA_Cells_Information_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _Served_EUTRA_Cells_Information_H_ */
#include <asn_internal.h>
