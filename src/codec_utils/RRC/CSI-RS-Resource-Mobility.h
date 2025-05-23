/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_CSI_RS_Resource_Mobility_H_
#define	_CSI_RS_Resource_Mobility_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CSI-RS-Index.h"
#include <NativeInteger.h>
#include <constr_CHOICE.h>
#include "SSB-Index.h"
#include <BOOLEAN.h>
#include <constr_SEQUENCE.h>
#include <BIT_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CSI_RS_Resource_Mobility__slotConfig_PR {
	CSI_RS_Resource_Mobility__slotConfig_PR_NOTHING,	/* No components present */
	CSI_RS_Resource_Mobility__slotConfig_PR_ms4,
	CSI_RS_Resource_Mobility__slotConfig_PR_ms5,
	CSI_RS_Resource_Mobility__slotConfig_PR_ms10,
	CSI_RS_Resource_Mobility__slotConfig_PR_ms20,
	CSI_RS_Resource_Mobility__slotConfig_PR_ms40
} CSI_RS_Resource_Mobility__slotConfig_PR;
typedef enum CSI_RS_Resource_Mobility__frequencyDomainAllocation_PR {
	CSI_RS_Resource_Mobility__frequencyDomainAllocation_PR_NOTHING,	/* No components present */
	CSI_RS_Resource_Mobility__frequencyDomainAllocation_PR_row1,
	CSI_RS_Resource_Mobility__frequencyDomainAllocation_PR_row2
} CSI_RS_Resource_Mobility__frequencyDomainAllocation_PR;

/* CSI-RS-Resource-Mobility */
typedef struct CSI_RS_Resource_Mobility {
	CSI_RS_Index_t	 csi_RS_Index;
	struct CSI_RS_Resource_Mobility__slotConfig {
		CSI_RS_Resource_Mobility__slotConfig_PR present;
		union CSI_RS_Resource_Mobility__slotConfig_u {
			long	 ms4;
			long	 ms5;
			long	 ms10;
			long	 ms20;
			long	 ms40;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} slotConfig;
	struct CSI_RS_Resource_Mobility__associatedSSB {
		SSB_Index_t	 ssb_Index;
		BOOLEAN_t	 isQuasiColocated;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *associatedSSB;
	struct CSI_RS_Resource_Mobility__frequencyDomainAllocation {
		CSI_RS_Resource_Mobility__frequencyDomainAllocation_PR present;
		union CSI_RS_Resource_Mobility__frequencyDomainAllocation_u {
			BIT_STRING_t	 row1;
			BIT_STRING_t	 row2;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} frequencyDomainAllocation;
	long	 firstOFDMSymbolInTimeDomain;
	long	 sequenceGenerationConfig;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CSI_RS_Resource_Mobility_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CSI_RS_Resource_Mobility;
extern asn_SEQUENCE_specifics_t asn_SPC_CSI_RS_Resource_Mobility_specs_1;
extern asn_TYPE_member_t asn_MBR_CSI_RS_Resource_Mobility_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _CSI_RS_Resource_Mobility_H_ */
#include <asn_internal.h>
