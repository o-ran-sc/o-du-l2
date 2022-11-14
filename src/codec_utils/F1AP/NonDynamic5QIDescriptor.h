/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_NonDynamic5QIDescriptor_H_
#define	_NonDynamic5QIDescriptor_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "AveragingWindow.h"
#include "MaxDataBurstVolume.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolExtensionContainer;

/* NonDynamic5QIDescriptor */
typedef struct NonDynamic5QIDescriptor {
	long	 fiveQI;
	long	*qoSPriorityLevel;	/* OPTIONAL */
	AveragingWindow_t	*averagingWindow;	/* OPTIONAL */
	MaxDataBurstVolume_t	*maxDataBurstVolume;	/* OPTIONAL */
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} NonDynamic5QIDescriptor_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_NonDynamic5QIDescriptor;
extern asn_SEQUENCE_specifics_t asn_SPC_NonDynamic5QIDescriptor_specs_1;
extern asn_TYPE_member_t asn_MBR_NonDynamic5QIDescriptor_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _NonDynamic5QIDescriptor_H_ */
#include <asn_internal.h>
