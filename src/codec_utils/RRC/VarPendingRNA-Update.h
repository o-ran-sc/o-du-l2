/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-UE-Variables"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_VarPendingRNA_Update_H_
#define	_VarPendingRNA_Update_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BOOLEAN.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* VarPendingRNA-Update */
typedef struct VarPendingRNA_Update {
	BOOLEAN_t	*pendingRNA_Update;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} VarPendingRNA_Update_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_VarPendingRNA_Update;

#ifdef __cplusplus
}
#endif

#endif	/* _VarPendingRNA_Update_H_ */
#include <asn_internal.h>
