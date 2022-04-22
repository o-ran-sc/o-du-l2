/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MAC_CellGroupConfig_H_
#define	_MAC_CellGroupConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BOOLEAN.h>
#include <NULL.h>
#include <constr_CHOICE.h>
#include "DataInactivityTimer.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MAC_CellGroupConfig__drx_Config_PR {
	MAC_CellGroupConfig__drx_Config_PR_NOTHING,	/* No components present */
	MAC_CellGroupConfig__drx_Config_PR_release,
	MAC_CellGroupConfig__drx_Config_PR_setup
} MAC_CellGroupConfig__drx_Config_PR;
typedef enum MAC_CellGroupConfig__phr_Config_PR {
	MAC_CellGroupConfig__phr_Config_PR_NOTHING,	/* No components present */
	MAC_CellGroupConfig__phr_Config_PR_release,
	MAC_CellGroupConfig__phr_Config_PR_setup
} MAC_CellGroupConfig__phr_Config_PR;
typedef enum MAC_CellGroupConfig__ext1__dataInactivityTimer_v1530_PR {
	MAC_CellGroupConfig__ext1__dataInactivityTimer_v1530_PR_NOTHING,	/* No components present */
	MAC_CellGroupConfig__ext1__dataInactivityTimer_v1530_PR_release,
	MAC_CellGroupConfig__ext1__dataInactivityTimer_v1530_PR_setup
} MAC_CellGroupConfig__ext1__dataInactivityTimer_v1530_PR;

/* Forward declarations */
struct SchedulingRequestConfig;
struct BSR_Config;
struct TAG_Config;
struct DRX_Config;
struct PHR_Config;

/* MAC-CellGroupConfig */
typedef struct MAC_CellGroupConfig {
	struct MAC_CellGroupConfig__drx_Config {
		MAC_CellGroupConfig__drx_Config_PR present;
		union MAC_CellGroupConfig__drx_Config_u {
			NULL_t	 release;
			struct DRX_Config	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *drx_Config;
	struct SchedulingRequestConfig	*schedulingRequestConfig;	/* OPTIONAL */
	struct BSR_Config	*bsr_Config;	/* OPTIONAL */
	struct TAG_Config	*tag_Config;	/* OPTIONAL */
	struct MAC_CellGroupConfig__phr_Config {
		MAC_CellGroupConfig__phr_Config_PR present;
		union MAC_CellGroupConfig__phr_Config_u {
			NULL_t	 release;
			struct PHR_Config	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *phr_Config;
	BOOLEAN_t	 skipUplinkTxDynamic;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct MAC_CellGroupConfig__ext1 {
		BOOLEAN_t	*csi_Mask_v1530;	/* OPTIONAL */
		struct MAC_CellGroupConfig__ext1__dataInactivityTimer_v1530 {
			MAC_CellGroupConfig__ext1__dataInactivityTimer_v1530_PR present;
			union MAC_CellGroupConfig__ext1__dataInactivityTimer_v1530_u {
				NULL_t	 release;
				DataInactivityTimer_t	 setup;
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *dataInactivityTimer_v1530;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MAC_CellGroupConfig_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MAC_CellGroupConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_MAC_CellGroupConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_MAC_CellGroupConfig_1[7];

#ifdef __cplusplus
}
#endif

#endif	/* _MAC_CellGroupConfig_H_ */
#include <asn_internal.h>
