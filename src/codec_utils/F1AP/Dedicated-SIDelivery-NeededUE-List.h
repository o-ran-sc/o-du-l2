/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Contents"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_Dedicated_SIDelivery_NeededUE_List_H_
#define	_Dedicated_SIDelivery_NeededUE_List_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolIE_SingleContainer;

/* Dedicated-SIDelivery-NeededUE-List */
typedef struct Dedicated_SIDelivery_NeededUE_List {
	A_SEQUENCE_OF(struct ProtocolIE_SingleContainer) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Dedicated_SIDelivery_NeededUE_List_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Dedicated_SIDelivery_NeededUE_List;

#ifdef __cplusplus
}
#endif

#endif	/* _Dedicated_SIDelivery_NeededUE_List_H_ */
#include <asn_internal.h>
