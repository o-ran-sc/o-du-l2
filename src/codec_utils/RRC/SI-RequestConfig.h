/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SI_RequestConfig_H_
#define	_SI_RequestConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include "RACH-ConfigGeneric.h"
#include <constr_SEQUENCE.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SI_RequestConfig__rach_OccasionsSI__ssb_perRACH_Occasion {
	SI_RequestConfig__rach_OccasionsSI__ssb_perRACH_Occasion_oneEighth	= 0,
	SI_RequestConfig__rach_OccasionsSI__ssb_perRACH_Occasion_oneFourth	= 1,
	SI_RequestConfig__rach_OccasionsSI__ssb_perRACH_Occasion_oneHalf	= 2,
	SI_RequestConfig__rach_OccasionsSI__ssb_perRACH_Occasion_one	= 3,
	SI_RequestConfig__rach_OccasionsSI__ssb_perRACH_Occasion_two	= 4,
	SI_RequestConfig__rach_OccasionsSI__ssb_perRACH_Occasion_four	= 5,
	SI_RequestConfig__rach_OccasionsSI__ssb_perRACH_Occasion_eight	= 6,
	SI_RequestConfig__rach_OccasionsSI__ssb_perRACH_Occasion_sixteen	= 7
} e_SI_RequestConfig__rach_OccasionsSI__ssb_perRACH_Occasion;
typedef enum SI_RequestConfig__si_RequestPeriod {
	SI_RequestConfig__si_RequestPeriod_one	= 0,
	SI_RequestConfig__si_RequestPeriod_two	= 1,
	SI_RequestConfig__si_RequestPeriod_four	= 2,
	SI_RequestConfig__si_RequestPeriod_six	= 3,
	SI_RequestConfig__si_RequestPeriod_eight	= 4,
	SI_RequestConfig__si_RequestPeriod_ten	= 5,
	SI_RequestConfig__si_RequestPeriod_twelve	= 6,
	SI_RequestConfig__si_RequestPeriod_sixteen	= 7
} e_SI_RequestConfig__si_RequestPeriod;

/* Forward declarations */
struct SI_RequestResources;

/* SI-RequestConfig */
typedef struct SI_RequestConfig {
	struct SI_RequestConfig__rach_OccasionsSI {
		RACH_ConfigGeneric_t	 rach_ConfigSI;
		long	 ssb_perRACH_Occasion;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *rach_OccasionsSI;
	long	*si_RequestPeriod;	/* OPTIONAL */
	struct SI_RequestConfig__si_RequestResources {
		A_SEQUENCE_OF(struct SI_RequestResources) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} si_RequestResources;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SI_RequestConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_ssb_perRACH_Occasion_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_si_RequestPeriod_13;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_SI_RequestConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_SI_RequestConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_SI_RequestConfig_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _SI_RequestConfig_H_ */
#include <asn_internal.h>
