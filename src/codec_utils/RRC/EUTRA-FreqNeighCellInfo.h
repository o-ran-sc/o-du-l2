/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_EUTRA_FreqNeighCellInfo_H_
#define	_EUTRA_FreqNeighCellInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "EUTRA-PhysCellId.h"
#include "EUTRA-Q-OffsetRange.h"
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EUTRA-FreqNeighCellInfo */
typedef struct EUTRA_FreqNeighCellInfo {
	EUTRA_PhysCellId_t	 physCellId;
	EUTRA_Q_OffsetRange_t	 q_OffsetCell;
	long	*q_RxLevMinOffsetCell;	/* OPTIONAL */
	long	*q_QualMinOffsetCell;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EUTRA_FreqNeighCellInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EUTRA_FreqNeighCellInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_EUTRA_FreqNeighCellInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_EUTRA_FreqNeighCellInfo_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _EUTRA_FreqNeighCellInfo_H_ */
#include <asn_internal.h>
