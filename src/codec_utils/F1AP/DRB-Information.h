/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_DRB_Information_H_
#define	_DRB_Information_H_


#include <asn_application.h>

/* Including external dependencies */
#include "QoSFlowLevelQoSParameters.h"
#include "SNSSAI.h"
#include "NotificationControl.h"
#include "Flows-Mapped-To-DRB-List.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolExtensionContainer;

/* DRB-Information */
typedef struct DRB_Information {
	QoSFlowLevelQoSParameters_t	 dRB_QoS;
	SNSSAI_t	 sNSSAI;
	NotificationControl_t	*notificationControl;	/* OPTIONAL */
	Flows_Mapped_To_DRB_List_t	 flows_Mapped_To_DRB_List;
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DRB_Information_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DRB_Information;
extern asn_SEQUENCE_specifics_t asn_SPC_DRB_Information_specs_1;
extern asn_TYPE_member_t asn_MBR_DRB_Information_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _DRB_Information_H_ */
#include <asn_internal.h>
