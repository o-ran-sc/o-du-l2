/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_RadioLinkMonitoringConfig_H_
#define	_RadioLinkMonitoringConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include "RadioLinkMonitoringRS-Id.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RadioLinkMonitoringConfig__beamFailureInstanceMaxCount {
	RadioLinkMonitoringConfig__beamFailureInstanceMaxCount_n1	= 0,
	RadioLinkMonitoringConfig__beamFailureInstanceMaxCount_n2	= 1,
	RadioLinkMonitoringConfig__beamFailureInstanceMaxCount_n3	= 2,
	RadioLinkMonitoringConfig__beamFailureInstanceMaxCount_n4	= 3,
	RadioLinkMonitoringConfig__beamFailureInstanceMaxCount_n5	= 4,
	RadioLinkMonitoringConfig__beamFailureInstanceMaxCount_n6	= 5,
	RadioLinkMonitoringConfig__beamFailureInstanceMaxCount_n8	= 6,
	RadioLinkMonitoringConfig__beamFailureInstanceMaxCount_n10	= 7
} e_RadioLinkMonitoringConfig__beamFailureInstanceMaxCount;
typedef enum RadioLinkMonitoringConfig__beamFailureDetectionTimer {
	RadioLinkMonitoringConfig__beamFailureDetectionTimer_pbfd1	= 0,
	RadioLinkMonitoringConfig__beamFailureDetectionTimer_pbfd2	= 1,
	RadioLinkMonitoringConfig__beamFailureDetectionTimer_pbfd3	= 2,
	RadioLinkMonitoringConfig__beamFailureDetectionTimer_pbfd4	= 3,
	RadioLinkMonitoringConfig__beamFailureDetectionTimer_pbfd5	= 4,
	RadioLinkMonitoringConfig__beamFailureDetectionTimer_pbfd6	= 5,
	RadioLinkMonitoringConfig__beamFailureDetectionTimer_pbfd8	= 6,
	RadioLinkMonitoringConfig__beamFailureDetectionTimer_pbfd10	= 7
} e_RadioLinkMonitoringConfig__beamFailureDetectionTimer;

/* Forward declarations */
struct RadioLinkMonitoringRS;

/* RadioLinkMonitoringConfig */
typedef struct RadioLinkMonitoringConfig {
	struct RadioLinkMonitoringConfig__failureDetectionResourcesToAddModList {
		A_SEQUENCE_OF(struct RadioLinkMonitoringRS) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *failureDetectionResourcesToAddModList;
	struct RadioLinkMonitoringConfig__failureDetectionResourcesToReleaseList {
		A_SEQUENCE_OF(RadioLinkMonitoringRS_Id_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *failureDetectionResourcesToReleaseList;
	long	*beamFailureInstanceMaxCount;	/* OPTIONAL */
	long	*beamFailureDetectionTimer;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RadioLinkMonitoringConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_beamFailureInstanceMaxCount_6;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_beamFailureDetectionTimer_15;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_RadioLinkMonitoringConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_RadioLinkMonitoringConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_RadioLinkMonitoringConfig_1[4];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RadioLinkMonitoringRS.h"

#endif	/* _RadioLinkMonitoringConfig_H_ */
#include <asn_internal.h>
