/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_ModulationOrder_H_
#define	_ModulationOrder_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ModulationOrder {
	ModulationOrder_bpsk_halfpi	= 0,
	ModulationOrder_bpsk	= 1,
	ModulationOrder_qpsk	= 2,
	ModulationOrder_qam16	= 3,
	ModulationOrder_qam64	= 4,
	ModulationOrder_qam256	= 5
} e_ModulationOrder;

/* ModulationOrder */
typedef long	 ModulationOrder_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_ModulationOrder_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ModulationOrder;
extern const asn_INTEGER_specifics_t asn_SPC_ModulationOrder_specs_1;
asn_struct_free_f ModulationOrder_free;
asn_struct_print_f ModulationOrder_print;
asn_constr_check_f ModulationOrder_constraint;
xer_type_decoder_f ModulationOrder_decode_xer;
xer_type_encoder_f ModulationOrder_encode_xer;
per_type_decoder_f ModulationOrder_decode_uper;
per_type_encoder_f ModulationOrder_encode_uper;
per_type_decoder_f ModulationOrder_decode_aper;
per_type_encoder_f ModulationOrder_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _ModulationOrder_H_ */
#include <asn_internal.h>
