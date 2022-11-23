/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_RateMatchPattern_H_
#define	_RateMatchPattern_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RateMatchPatternId.h"
#include "SubcarrierSpacing.h"
#include <NativeEnumerated.h>
#include "ControlResourceSetId.h"
#include <BIT_STRING.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RateMatchPattern__patternType_PR {
	RateMatchPattern__patternType_PR_NOTHING,	/* No components present */
	RateMatchPattern__patternType_PR_bitmaps,
	RateMatchPattern__patternType_PR_controlResourceSet
} RateMatchPattern__patternType_PR;
typedef enum RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock_PR {
	RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock_PR_NOTHING,	/* No components present */
	RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock_PR_oneSlot,
	RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock_PR_twoSlots
} RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock_PR;
typedef enum RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR {
	RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_NOTHING,	/* No components present */
	RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n2,
	RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n4,
	RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n5,
	RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n8,
	RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n10,
	RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n20,
	RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n40
} RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR;
typedef enum RateMatchPattern__dummy {
	RateMatchPattern__dummy_dynamic	= 0,
	RateMatchPattern__dummy_semiStatic	= 1
} e_RateMatchPattern__dummy;

/* RateMatchPattern */
typedef struct RateMatchPattern {
	RateMatchPatternId_t	 rateMatchPatternId;
	struct RateMatchPattern__patternType {
		RateMatchPattern__patternType_PR present;
		union RateMatchPattern__patternType_u {
			struct RateMatchPattern__patternType__bitmaps {
				BIT_STRING_t	 resourceBlocks;
				struct RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock {
					RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock_PR present;
					union RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock_u {
						BIT_STRING_t	 oneSlot;
						BIT_STRING_t	 twoSlots;
					} choice;
					
					/* Context for parsing across buffer boundaries */
					asn_struct_ctx_t _asn_ctx;
				} symbolsInResourceBlock;
				struct RateMatchPattern__patternType__bitmaps__periodicityAndPattern {
					RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR present;
					union RateMatchPattern__patternType__bitmaps__periodicityAndPattern_u {
						BIT_STRING_t	 n2;
						BIT_STRING_t	 n4;
						BIT_STRING_t	 n5;
						BIT_STRING_t	 n8;
						BIT_STRING_t	 n10;
						BIT_STRING_t	 n20;
						BIT_STRING_t	 n40;
					} choice;
					
					/* Context for parsing across buffer boundaries */
					asn_struct_ctx_t _asn_ctx;
				} *periodicityAndPattern;
				/*
				 * This type is extensible,
				 * possible extensions are below.
				 */
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *bitmaps;
			ControlResourceSetId_t	 controlResourceSet;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} patternType;
	SubcarrierSpacing_t	*subcarrierSpacing;	/* OPTIONAL */
	long	 dummy;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RateMatchPattern_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_dummy_20;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_RateMatchPattern;
extern asn_SEQUENCE_specifics_t asn_SPC_RateMatchPattern_specs_1;
extern asn_TYPE_member_t asn_MBR_RateMatchPattern_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _RateMatchPattern_H_ */
#include <asn_internal.h>
