/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "F1.asn1"
 * 	`asn1c -D ./out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CUtoDURRCInformation_H_
#define	_CUtoDURRCInformation_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CG-ConfigInfo.h"
#include "UE-CapabilityRAT-ContainerList.h"
#include "MeasConfig.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolExtensionContainer;

/* CUtoDURRCInformation */
typedef struct CUtoDURRCInformation {
	CG_ConfigInfo_t	*cG_ConfigInfo;	/* OPTIONAL */
	UE_CapabilityRAT_ContainerList_t	*uE_CapabilityRAT_ContainerList;	/* OPTIONAL */
	MeasConfig_t	*measConfig;	/* OPTIONAL */
	struct ProtocolExtensionContainer_4624P16 *iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CUtoDURRCInformation_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CUtoDURRCInformation;

#ifdef __cplusplus
}
#endif

#endif	/* _CUtoDURRCInformation_H_ */
#include <asn_internal.h>
