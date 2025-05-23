/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_BWP_UplinkDedicated_H_
#define	_BWP_UplinkDedicated_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BWP_UplinkDedicated__pucch_Config_PR {
	BWP_UplinkDedicated__pucch_Config_PR_NOTHING,	/* No components present */
	BWP_UplinkDedicated__pucch_Config_PR_release,
	BWP_UplinkDedicated__pucch_Config_PR_setup
} BWP_UplinkDedicated__pucch_Config_PR;
typedef enum BWP_UplinkDedicated__pusch_Config_PR {
	BWP_UplinkDedicated__pusch_Config_PR_NOTHING,	/* No components present */
	BWP_UplinkDedicated__pusch_Config_PR_release,
	BWP_UplinkDedicated__pusch_Config_PR_setup
} BWP_UplinkDedicated__pusch_Config_PR;
typedef enum BWP_UplinkDedicated__configuredGrantConfig_PR {
	BWP_UplinkDedicated__configuredGrantConfig_PR_NOTHING,	/* No components present */
	BWP_UplinkDedicated__configuredGrantConfig_PR_release,
	BWP_UplinkDedicated__configuredGrantConfig_PR_setup
} BWP_UplinkDedicated__configuredGrantConfig_PR;
typedef enum BWP_UplinkDedicated__srs_Config_PR {
	BWP_UplinkDedicated__srs_Config_PR_NOTHING,	/* No components present */
	BWP_UplinkDedicated__srs_Config_PR_release,
	BWP_UplinkDedicated__srs_Config_PR_setup
} BWP_UplinkDedicated__srs_Config_PR;
typedef enum BWP_UplinkDedicated__beamFailureRecoveryConfig_PR {
	BWP_UplinkDedicated__beamFailureRecoveryConfig_PR_NOTHING,	/* No components present */
	BWP_UplinkDedicated__beamFailureRecoveryConfig_PR_release,
	BWP_UplinkDedicated__beamFailureRecoveryConfig_PR_setup
} BWP_UplinkDedicated__beamFailureRecoveryConfig_PR;

/* Forward declarations */
struct PUCCH_Config;
struct PUSCH_Config;
struct ConfiguredGrantConfig;
struct SRS_Config;
struct BeamFailureRecoveryConfig;

/* BWP-UplinkDedicated */
typedef struct BWP_UplinkDedicated {
	struct BWP_UplinkDedicated__pucch_Config {
		BWP_UplinkDedicated__pucch_Config_PR present;
		union BWP_UplinkDedicated__pucch_Config_u {
			NULL_t	 release;
			struct PUCCH_Config	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *pucch_Config;
	struct BWP_UplinkDedicated__pusch_Config {
		BWP_UplinkDedicated__pusch_Config_PR present;
		union BWP_UplinkDedicated__pusch_Config_u {
			NULL_t	 release;
			struct PUSCH_Config	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *pusch_Config;
	struct BWP_UplinkDedicated__configuredGrantConfig {
		BWP_UplinkDedicated__configuredGrantConfig_PR present;
		union BWP_UplinkDedicated__configuredGrantConfig_u {
			NULL_t	 release;
			struct ConfiguredGrantConfig	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *configuredGrantConfig;
	struct BWP_UplinkDedicated__srs_Config {
		BWP_UplinkDedicated__srs_Config_PR present;
		union BWP_UplinkDedicated__srs_Config_u {
			NULL_t	 release;
			struct SRS_Config	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *srs_Config;
	struct BWP_UplinkDedicated__beamFailureRecoveryConfig {
		BWP_UplinkDedicated__beamFailureRecoveryConfig_PR present;
		union BWP_UplinkDedicated__beamFailureRecoveryConfig_u {
			NULL_t	 release;
			struct BeamFailureRecoveryConfig	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *beamFailureRecoveryConfig;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BWP_UplinkDedicated_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BWP_UplinkDedicated;
extern asn_SEQUENCE_specifics_t asn_SPC_BWP_UplinkDedicated_specs_1;
extern asn_TYPE_member_t asn_MBR_BWP_UplinkDedicated_1[5];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "PUCCH-Config.h"
#include "PUSCH-Config.h"
#include "ConfiguredGrantConfig.h"
#include "SRS-Config.h"
#include "BeamFailureRecoveryConfig.h"

#endif	/* _BWP_UplinkDedicated_H_ */
#include <asn_internal.h>
