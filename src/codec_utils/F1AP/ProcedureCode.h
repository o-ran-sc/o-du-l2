/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-CommonDataTypes"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_ProcedureCode_H_
#define	_ProcedureCode_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ProcedureCode */
typedef long	 ProcedureCode_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_ProcedureCode_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ProcedureCode;
asn_struct_free_f ProcedureCode_free;
asn_struct_print_f ProcedureCode_print;
asn_constr_check_f ProcedureCode_constraint;
ber_type_decoder_f ProcedureCode_decode_ber;
der_type_encoder_f ProcedureCode_encode_der;
xer_type_decoder_f ProcedureCode_decode_xer;
xer_type_encoder_f ProcedureCode_encode_xer;
oer_type_decoder_f ProcedureCode_decode_oer;
oer_type_encoder_f ProcedureCode_encode_oer;
per_type_decoder_f ProcedureCode_decode_uper;
per_type_encoder_f ProcedureCode_encode_uper;
per_type_decoder_f ProcedureCode_decode_aper;
per_type_encoder_f ProcedureCode_encode_aper;
#define ProcedureCode_id_Reset	((ProcedureCode_t)0)
#define ProcedureCode_id_F1Setup	((ProcedureCode_t)1)
#define ProcedureCode_id_ErrorIndication	((ProcedureCode_t)2)
#define ProcedureCode_id_gNBDUConfigurationUpdate	((ProcedureCode_t)3)
#define ProcedureCode_id_gNBCUConfigurationUpdate	((ProcedureCode_t)4)
#define ProcedureCode_id_UEContextSetup	((ProcedureCode_t)5)
#define ProcedureCode_id_UEContextRelease	((ProcedureCode_t)6)
#define ProcedureCode_id_UEContextModification	((ProcedureCode_t)7)
#define ProcedureCode_id_UEContextModificationRequired	((ProcedureCode_t)8)
#define ProcedureCode_id_UEMobilityCommand	((ProcedureCode_t)9)
#define ProcedureCode_id_UEContextReleaseRequest	((ProcedureCode_t)10)
#define ProcedureCode_id_InitialULRRCMessageTransfer	((ProcedureCode_t)11)
#define ProcedureCode_id_DLRRCMessageTransfer	((ProcedureCode_t)12)
#define ProcedureCode_id_ULRRCMessageTransfer	((ProcedureCode_t)13)
#define ProcedureCode_id_privateMessage	((ProcedureCode_t)14)
#define ProcedureCode_id_UEInactivityNotification	((ProcedureCode_t)15)
#define ProcedureCode_id_GNBDUResourceCoordination	((ProcedureCode_t)16)
#define ProcedureCode_id_SystemInformationDeliveryCommand	((ProcedureCode_t)17)
#define ProcedureCode_id_Paging	((ProcedureCode_t)18)
#define ProcedureCode_id_Notify	((ProcedureCode_t)19)
#define ProcedureCode_id_WriteReplaceWarning	((ProcedureCode_t)20)
#define ProcedureCode_id_PWSCancel	((ProcedureCode_t)21)
#define ProcedureCode_id_PWSRestartIndication	((ProcedureCode_t)22)
#define ProcedureCode_id_PWSFailureIndication	((ProcedureCode_t)23)
#define ProcedureCode_id_GNBDUStatusIndication	((ProcedureCode_t)24)
#define ProcedureCode_id_RRCDeliveryReport	((ProcedureCode_t)25)

#ifdef __cplusplus
}
#endif

#endif	/* _ProcedureCode_H_ */
#include <asn_internal.h>
