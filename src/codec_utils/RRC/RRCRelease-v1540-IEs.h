/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RRCRelease_v1540_IEs_H_
#define	_RRCRelease_v1540_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RejectWaitTime.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RRCRelease-v1540-IEs */
typedef struct RRCRelease_v1540_IEs {
	RejectWaitTime_t	*waitTime;	/* OPTIONAL */
	struct RRCRelease_v1540_IEs__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRCRelease_v1540_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RRCRelease_v1540_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_RRCRelease_v1540_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_RRCRelease_v1540_IEs_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _RRCRelease_v1540_IEs_H_ */
#include <asn_internal.h>
