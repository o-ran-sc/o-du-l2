/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_KPM_v2.03.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#ifndef	_RIC_ReportStyle_Item_H_
#define	_RIC_ReportStyle_Item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RIC-Style-Type.h"
#include "RIC-Style-Name.h"
#include "RIC-Format-Type.h"
#include "MeasurementInfo-Action-List.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RIC-ReportStyle-Item */
typedef struct RIC_ReportStyle_Item {
	RIC_Style_Type_t	 ric_ReportStyle_Type;
	RIC_Style_Name_t	 ric_ReportStyle_Name;
	RIC_Format_Type_t	 ric_ActionFormat_Type;
	MeasurementInfo_Action_List_t	 measInfo_Action_List;
	RIC_Format_Type_t	 ric_IndicationHeaderFormat_Type;
	RIC_Format_Type_t	 ric_IndicationMessageFormat_Type;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RIC_ReportStyle_Item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RIC_ReportStyle_Item;
extern asn_SEQUENCE_specifics_t asn_SPC_RIC_ReportStyle_Item_specs_1;
extern asn_TYPE_member_t asn_MBR_RIC_ReportStyle_Item_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _RIC_ReportStyle_Item_H_ */
#include <asn_internal.h>
