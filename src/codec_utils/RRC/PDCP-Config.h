/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PDCP_Config_H_
#define	_PDCP_Config_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NULL.h>
#include <NativeInteger.h>
#include <BOOLEAN.h>
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>
#include "UL-DataSplitThreshold.h"
#include "CellGroupId.h"
#include "LogicalChannelIdentity.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PDCP_Config__drb__discardTimer {
	PDCP_Config__drb__discardTimer_ms10	= 0,
	PDCP_Config__drb__discardTimer_ms20	= 1,
	PDCP_Config__drb__discardTimer_ms30	= 2,
	PDCP_Config__drb__discardTimer_ms40	= 3,
	PDCP_Config__drb__discardTimer_ms50	= 4,
	PDCP_Config__drb__discardTimer_ms60	= 5,
	PDCP_Config__drb__discardTimer_ms75	= 6,
	PDCP_Config__drb__discardTimer_ms100	= 7,
	PDCP_Config__drb__discardTimer_ms150	= 8,
	PDCP_Config__drb__discardTimer_ms200	= 9,
	PDCP_Config__drb__discardTimer_ms250	= 10,
	PDCP_Config__drb__discardTimer_ms300	= 11,
	PDCP_Config__drb__discardTimer_ms500	= 12,
	PDCP_Config__drb__discardTimer_ms750	= 13,
	PDCP_Config__drb__discardTimer_ms1500	= 14,
	PDCP_Config__drb__discardTimer_infinity	= 15
} e_PDCP_Config__drb__discardTimer;
typedef enum PDCP_Config__drb__pdcp_SN_SizeUL {
	PDCP_Config__drb__pdcp_SN_SizeUL_len12bits	= 0,
	PDCP_Config__drb__pdcp_SN_SizeUL_len18bits	= 1
} e_PDCP_Config__drb__pdcp_SN_SizeUL;
typedef enum PDCP_Config__drb__pdcp_SN_SizeDL {
	PDCP_Config__drb__pdcp_SN_SizeDL_len12bits	= 0,
	PDCP_Config__drb__pdcp_SN_SizeDL_len18bits	= 1
} e_PDCP_Config__drb__pdcp_SN_SizeDL;
typedef enum PDCP_Config__drb__headerCompression_PR {
	PDCP_Config__drb__headerCompression_PR_NOTHING,	/* No components present */
	PDCP_Config__drb__headerCompression_PR_notUsed,
	PDCP_Config__drb__headerCompression_PR_rohc,
	PDCP_Config__drb__headerCompression_PR_uplinkOnlyROHC
	/* Extensions may appear below */
	
} PDCP_Config__drb__headerCompression_PR;
typedef enum PDCP_Config__drb__headerCompression__rohc__drb_ContinueROHC {
	PDCP_Config__drb__headerCompression__rohc__drb_ContinueROHC_true	= 0
} e_PDCP_Config__drb__headerCompression__rohc__drb_ContinueROHC;
typedef enum PDCP_Config__drb__headerCompression__uplinkOnlyROHC__drb_ContinueROHC {
	PDCP_Config__drb__headerCompression__uplinkOnlyROHC__drb_ContinueROHC_true	= 0
} e_PDCP_Config__drb__headerCompression__uplinkOnlyROHC__drb_ContinueROHC;
typedef enum PDCP_Config__drb__integrityProtection {
	PDCP_Config__drb__integrityProtection_enabled	= 0
} e_PDCP_Config__drb__integrityProtection;
typedef enum PDCP_Config__drb__statusReportRequired {
	PDCP_Config__drb__statusReportRequired_true	= 0
} e_PDCP_Config__drb__statusReportRequired;
typedef enum PDCP_Config__drb__outOfOrderDelivery {
	PDCP_Config__drb__outOfOrderDelivery_true	= 0
} e_PDCP_Config__drb__outOfOrderDelivery;
typedef enum PDCP_Config__t_Reordering {
	PDCP_Config__t_Reordering_ms0	= 0,
	PDCP_Config__t_Reordering_ms1	= 1,
	PDCP_Config__t_Reordering_ms2	= 2,
	PDCP_Config__t_Reordering_ms4	= 3,
	PDCP_Config__t_Reordering_ms5	= 4,
	PDCP_Config__t_Reordering_ms8	= 5,
	PDCP_Config__t_Reordering_ms10	= 6,
	PDCP_Config__t_Reordering_ms15	= 7,
	PDCP_Config__t_Reordering_ms20	= 8,
	PDCP_Config__t_Reordering_ms30	= 9,
	PDCP_Config__t_Reordering_ms40	= 10,
	PDCP_Config__t_Reordering_ms50	= 11,
	PDCP_Config__t_Reordering_ms60	= 12,
	PDCP_Config__t_Reordering_ms80	= 13,
	PDCP_Config__t_Reordering_ms100	= 14,
	PDCP_Config__t_Reordering_ms120	= 15,
	PDCP_Config__t_Reordering_ms140	= 16,
	PDCP_Config__t_Reordering_ms160	= 17,
	PDCP_Config__t_Reordering_ms180	= 18,
	PDCP_Config__t_Reordering_ms200	= 19,
	PDCP_Config__t_Reordering_ms220	= 20,
	PDCP_Config__t_Reordering_ms240	= 21,
	PDCP_Config__t_Reordering_ms260	= 22,
	PDCP_Config__t_Reordering_ms280	= 23,
	PDCP_Config__t_Reordering_ms300	= 24,
	PDCP_Config__t_Reordering_ms500	= 25,
	PDCP_Config__t_Reordering_ms750	= 26,
	PDCP_Config__t_Reordering_ms1000	= 27,
	PDCP_Config__t_Reordering_ms1250	= 28,
	PDCP_Config__t_Reordering_ms1500	= 29,
	PDCP_Config__t_Reordering_ms1750	= 30,
	PDCP_Config__t_Reordering_ms2000	= 31,
	PDCP_Config__t_Reordering_ms2250	= 32,
	PDCP_Config__t_Reordering_ms2500	= 33,
	PDCP_Config__t_Reordering_ms2750	= 34,
	PDCP_Config__t_Reordering_ms3000	= 35,
	PDCP_Config__t_Reordering_spare28	= 36,
	PDCP_Config__t_Reordering_spare27	= 37,
	PDCP_Config__t_Reordering_spare26	= 38,
	PDCP_Config__t_Reordering_spare25	= 39,
	PDCP_Config__t_Reordering_spare24	= 40,
	PDCP_Config__t_Reordering_spare23	= 41,
	PDCP_Config__t_Reordering_spare22	= 42,
	PDCP_Config__t_Reordering_spare21	= 43,
	PDCP_Config__t_Reordering_spare20	= 44,
	PDCP_Config__t_Reordering_spare19	= 45,
	PDCP_Config__t_Reordering_spare18	= 46,
	PDCP_Config__t_Reordering_spare17	= 47,
	PDCP_Config__t_Reordering_spare16	= 48,
	PDCP_Config__t_Reordering_spare15	= 49,
	PDCP_Config__t_Reordering_spare14	= 50,
	PDCP_Config__t_Reordering_spare13	= 51,
	PDCP_Config__t_Reordering_spare12	= 52,
	PDCP_Config__t_Reordering_spare11	= 53,
	PDCP_Config__t_Reordering_spare10	= 54,
	PDCP_Config__t_Reordering_spare09	= 55,
	PDCP_Config__t_Reordering_spare08	= 56,
	PDCP_Config__t_Reordering_spare07	= 57,
	PDCP_Config__t_Reordering_spare06	= 58,
	PDCP_Config__t_Reordering_spare05	= 59,
	PDCP_Config__t_Reordering_spare04	= 60,
	PDCP_Config__t_Reordering_spare03	= 61,
	PDCP_Config__t_Reordering_spare02	= 62,
	PDCP_Config__t_Reordering_spare01	= 63
} e_PDCP_Config__t_Reordering;
typedef enum PDCP_Config__ext1__cipheringDisabled {
	PDCP_Config__ext1__cipheringDisabled_true	= 0
} e_PDCP_Config__ext1__cipheringDisabled;

/* PDCP-Config */
typedef struct PDCP_Config {
	struct PDCP_Config__drb {
		long	*discardTimer;	/* OPTIONAL */
		long	*pdcp_SN_SizeUL;	/* OPTIONAL */
		long	*pdcp_SN_SizeDL;	/* OPTIONAL */
		struct PDCP_Config__drb__headerCompression {
			PDCP_Config__drb__headerCompression_PR present;
			union PDCP_Config__drb__headerCompression_u {
				NULL_t	 notUsed;
				struct PDCP_Config__drb__headerCompression__rohc {
					long	*maxCID;	/* DEFAULT 15 */
					struct PDCP_Config__drb__headerCompression__rohc__profiles {
						BOOLEAN_t	 profile0x0001;
						BOOLEAN_t	 profile0x0002;
						BOOLEAN_t	 profile0x0003;
						BOOLEAN_t	 profile0x0004;
						BOOLEAN_t	 profile0x0006;
						BOOLEAN_t	 profile0x0101;
						BOOLEAN_t	 profile0x0102;
						BOOLEAN_t	 profile0x0103;
						BOOLEAN_t	 profile0x0104;
						
						/* Context for parsing across buffer boundaries */
						asn_struct_ctx_t _asn_ctx;
					} profiles;
					long	*drb_ContinueROHC;	/* OPTIONAL */
					
					/* Context for parsing across buffer boundaries */
					asn_struct_ctx_t _asn_ctx;
				} *rohc;
				struct PDCP_Config__drb__headerCompression__uplinkOnlyROHC {
					long	*maxCID;	/* DEFAULT 15 */
					struct PDCP_Config__drb__headerCompression__uplinkOnlyROHC__profiles {
						BOOLEAN_t	 profile0x0006;
						
						/* Context for parsing across buffer boundaries */
						asn_struct_ctx_t _asn_ctx;
					} profiles;
					long	*drb_ContinueROHC;	/* OPTIONAL */
					
					/* Context for parsing across buffer boundaries */
					asn_struct_ctx_t _asn_ctx;
				} *uplinkOnlyROHC;
				/*
				 * This type is extensible,
				 * possible extensions are below.
				 */
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} headerCompression;
		long	*integrityProtection;	/* OPTIONAL */
		long	*statusReportRequired;	/* OPTIONAL */
		long	*outOfOrderDelivery;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *drb;
	struct PDCP_Config__moreThanOneRLC {
		struct PDCP_Config__moreThanOneRLC__primaryPath {
			CellGroupId_t	*cellGroup;	/* OPTIONAL */
			LogicalChannelIdentity_t	*logicalChannel;	/* OPTIONAL */
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} primaryPath;
		UL_DataSplitThreshold_t	*ul_DataSplitThreshold;	/* OPTIONAL */
		BOOLEAN_t	*pdcp_Duplication;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *moreThanOneRLC;
	long	*t_Reordering;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct PDCP_Config__ext1 {
		long	*cipheringDisabled;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PDCP_Config_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_discardTimer_3;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pdcp_SN_SizeUL_20;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pdcp_SN_SizeDL_23;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_drb_ContinueROHC_40;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_drb_ContinueROHC_46;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_integrityProtection_49;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_statusReportRequired_51;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_outOfOrderDelivery_53;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_t_Reordering_61;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_cipheringDisabled_128;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PDCP_Config;
extern asn_SEQUENCE_specifics_t asn_SPC_PDCP_Config_specs_1;
extern asn_TYPE_member_t asn_MBR_PDCP_Config_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _PDCP_Config_H_ */
#include <asn_internal.h>
