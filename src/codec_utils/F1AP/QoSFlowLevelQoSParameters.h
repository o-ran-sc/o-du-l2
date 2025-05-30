/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_QoSFlowLevelQoSParameters_H_
#define	_QoSFlowLevelQoSParameters_H_


#include <asn_application.h>

/* Including external dependencies */
#include "QoS-Characteristics.h"
#include "NGRANAllocationAndRetentionPriority.h"
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum QoSFlowLevelQoSParameters__reflective_QoS_Attribute {
	QoSFlowLevelQoSParameters__reflective_QoS_Attribute_subject_to	= 0
	/*
	 * Enumeration is extensible
	 */
} e_QoSFlowLevelQoSParameters__reflective_QoS_Attribute;

/* Forward declarations */
struct GBR_QoSFlowInformation;
struct ProtocolExtensionContainer;

/* QoSFlowLevelQoSParameters */
typedef struct QoSFlowLevelQoSParameters {
	QoS_Characteristics_t	 qoS_Characteristics;
	NGRANAllocationAndRetentionPriority_t	 nGRANallocationRetentionPriority;
	struct GBR_QoSFlowInformation	*gBR_QoS_Flow_Information;	/* OPTIONAL */
	long	*reflective_QoS_Attribute;	/* OPTIONAL */
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} QoSFlowLevelQoSParameters_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_reflective_QoS_Attribute_5;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_QoSFlowLevelQoSParameters;
extern asn_SEQUENCE_specifics_t asn_SPC_QoSFlowLevelQoSParameters_specs_1;
extern asn_TYPE_member_t asn_MBR_QoSFlowLevelQoSParameters_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _QoSFlowLevelQoSParameters_H_ */
#include <asn_internal.h>
