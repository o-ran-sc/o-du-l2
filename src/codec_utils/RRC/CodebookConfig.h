/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CodebookConfig_H_
#define	_CodebookConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <BIT_STRING.h>
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>
#include <NativeEnumerated.h>
#include <BOOLEAN.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CodebookConfig__codebookType_PR {
	CodebookConfig__codebookType_PR_NOTHING,	/* No components present */
	CodebookConfig__codebookType_PR_type1,
	CodebookConfig__codebookType_PR_type2
} CodebookConfig__codebookType_PR;
typedef enum CodebookConfig__codebookType__type1__subType_PR {
	CodebookConfig__codebookType__type1__subType_PR_NOTHING,	/* No components present */
	CodebookConfig__codebookType__type1__subType_PR_typeI_SinglePanel,
	CodebookConfig__codebookType__type1__subType_PR_typeI_MultiPanel
} CodebookConfig__codebookType__type1__subType_PR;
typedef enum CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts_PR {
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts_PR_NOTHING,	/* No components present */
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts_PR_two,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts_PR_moreThanTwo
} CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts_PR;
typedef enum CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR {
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_NOTHING,	/* No components present */
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_two_one_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_two_two_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_four_one_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_three_two_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_six_one_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_four_two_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_eight_one_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_four_three_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_six_two_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_twelve_one_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_four_four_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_eight_two_TypeI_SinglePanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR_sixteen_one_TypeI_SinglePanel_Restriction
} CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR;
typedef enum CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR {
	CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR_NOTHING,	/* No components present */
	CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR_two_two_one_TypeI_MultiPanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR_two_four_one_TypeI_MultiPanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR_four_two_one_TypeI_MultiPanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR_two_two_two_TypeI_MultiPanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR_two_eight_one_TypeI_MultiPanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR_four_four_one_TypeI_MultiPanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR_two_four_two_TypeI_MultiPanel_Restriction,
	CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR_four_two_two_TypeI_MultiPanel_Restriction
} CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR;
typedef enum CodebookConfig__codebookType__type2__subType_PR {
	CodebookConfig__codebookType__type2__subType_PR_NOTHING,	/* No components present */
	CodebookConfig__codebookType__type2__subType_PR_typeII,
	CodebookConfig__codebookType__type2__subType_PR_typeII_PortSelection
} CodebookConfig__codebookType__type2__subType_PR;
typedef enum CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR {
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_NOTHING,	/* No components present */
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_two_one,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_two_two,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_four_one,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_three_two,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_six_one,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_four_two,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_eight_one,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_four_three,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_six_two,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_twelve_one,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_four_four,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_eight_two,
	CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR_sixteen_one
} CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR;
typedef enum CodebookConfig__codebookType__type2__subType__typeII_PortSelection__portSelectionSamplingSize {
	CodebookConfig__codebookType__type2__subType__typeII_PortSelection__portSelectionSamplingSize_n1	= 0,
	CodebookConfig__codebookType__type2__subType__typeII_PortSelection__portSelectionSamplingSize_n2	= 1,
	CodebookConfig__codebookType__type2__subType__typeII_PortSelection__portSelectionSamplingSize_n3	= 2,
	CodebookConfig__codebookType__type2__subType__typeII_PortSelection__portSelectionSamplingSize_n4	= 3
} e_CodebookConfig__codebookType__type2__subType__typeII_PortSelection__portSelectionSamplingSize;
typedef enum CodebookConfig__codebookType__type2__phaseAlphabetSize {
	CodebookConfig__codebookType__type2__phaseAlphabetSize_n4	= 0,
	CodebookConfig__codebookType__type2__phaseAlphabetSize_n8	= 1
} e_CodebookConfig__codebookType__type2__phaseAlphabetSize;
typedef enum CodebookConfig__codebookType__type2__numberOfBeams {
	CodebookConfig__codebookType__type2__numberOfBeams_two	= 0,
	CodebookConfig__codebookType__type2__numberOfBeams_three	= 1,
	CodebookConfig__codebookType__type2__numberOfBeams_four	= 2
} e_CodebookConfig__codebookType__type2__numberOfBeams;

/* CodebookConfig */
typedef struct CodebookConfig {
	struct CodebookConfig__codebookType {
		CodebookConfig__codebookType_PR present;
		union CodebookConfig__codebookType_u {
			struct CodebookConfig__codebookType__type1 {
				struct CodebookConfig__codebookType__type1__subType {
					CodebookConfig__codebookType__type1__subType_PR present;
					union CodebookConfig__codebookType__type1__subType_u {
						struct CodebookConfig__codebookType__type1__subType__typeI_SinglePanel {
							struct CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts {
								CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts_PR present;
								union CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts_u {
									struct CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__two {
										BIT_STRING_t	 twoTX_CodebookSubsetRestriction;
										
										/* Context for parsing across buffer boundaries */
										asn_struct_ctx_t _asn_ctx;
									} *two;
									struct CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo {
										struct CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2 {
											CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_PR present;
											union CodebookConfig__codebookType__type1__subType__typeI_SinglePanel__nrOfAntennaPorts__moreThanTwo__n1_n2_u {
												BIT_STRING_t	 two_one_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 two_two_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 four_one_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 three_two_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 six_one_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 four_two_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 eight_one_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 four_three_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 six_two_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 twelve_one_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 four_four_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 eight_two_TypeI_SinglePanel_Restriction;
												BIT_STRING_t	 sixteen_one_TypeI_SinglePanel_Restriction;
											} choice;
											
											/* Context for parsing across buffer boundaries */
											asn_struct_ctx_t _asn_ctx;
										} n1_n2;
										BIT_STRING_t	*typeI_SinglePanel_codebookSubsetRestriction_i2;	/* OPTIONAL */
										
										/* Context for parsing across buffer boundaries */
										asn_struct_ctx_t _asn_ctx;
									} *moreThanTwo;
								} choice;
								
								/* Context for parsing across buffer boundaries */
								asn_struct_ctx_t _asn_ctx;
							} nrOfAntennaPorts;
							BIT_STRING_t	 typeI_SinglePanel_ri_Restriction;
							
							/* Context for parsing across buffer boundaries */
							asn_struct_ctx_t _asn_ctx;
						} *typeI_SinglePanel;
						struct CodebookConfig__codebookType__type1__subType__typeI_MultiPanel {
							struct CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2 {
								CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_PR present;
								union CodebookConfig__codebookType__type1__subType__typeI_MultiPanel__ng_n1_n2_u {
									BIT_STRING_t	 two_two_one_TypeI_MultiPanel_Restriction;
									BIT_STRING_t	 two_four_one_TypeI_MultiPanel_Restriction;
									BIT_STRING_t	 four_two_one_TypeI_MultiPanel_Restriction;
									BIT_STRING_t	 two_two_two_TypeI_MultiPanel_Restriction;
									BIT_STRING_t	 two_eight_one_TypeI_MultiPanel_Restriction;
									BIT_STRING_t	 four_four_one_TypeI_MultiPanel_Restriction;
									BIT_STRING_t	 two_four_two_TypeI_MultiPanel_Restriction;
									BIT_STRING_t	 four_two_two_TypeI_MultiPanel_Restriction;
								} choice;
								
								/* Context for parsing across buffer boundaries */
								asn_struct_ctx_t _asn_ctx;
							} ng_n1_n2;
							BIT_STRING_t	 ri_Restriction;
							
							/* Context for parsing across buffer boundaries */
							asn_struct_ctx_t _asn_ctx;
						} *typeI_MultiPanel;
					} choice;
					
					/* Context for parsing across buffer boundaries */
					asn_struct_ctx_t _asn_ctx;
				} subType;
				long	 codebookMode;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *type1;
			struct CodebookConfig__codebookType__type2 {
				struct CodebookConfig__codebookType__type2__subType {
					CodebookConfig__codebookType__type2__subType_PR present;
					union CodebookConfig__codebookType__type2__subType_u {
						struct CodebookConfig__codebookType__type2__subType__typeII {
							struct CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction {
								CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_PR present;
								union CodebookConfig__codebookType__type2__subType__typeII__n1_n2_codebookSubsetRestriction_u {
									BIT_STRING_t	 two_one;
									BIT_STRING_t	 two_two;
									BIT_STRING_t	 four_one;
									BIT_STRING_t	 three_two;
									BIT_STRING_t	 six_one;
									BIT_STRING_t	 four_two;
									BIT_STRING_t	 eight_one;
									BIT_STRING_t	 four_three;
									BIT_STRING_t	 six_two;
									BIT_STRING_t	 twelve_one;
									BIT_STRING_t	 four_four;
									BIT_STRING_t	 eight_two;
									BIT_STRING_t	 sixteen_one;
								} choice;
								
								/* Context for parsing across buffer boundaries */
								asn_struct_ctx_t _asn_ctx;
							} n1_n2_codebookSubsetRestriction;
							BIT_STRING_t	 typeII_RI_Restriction;
							
							/* Context for parsing across buffer boundaries */
							asn_struct_ctx_t _asn_ctx;
						} *typeII;
						struct CodebookConfig__codebookType__type2__subType__typeII_PortSelection {
							long	*portSelectionSamplingSize;	/* OPTIONAL */
							BIT_STRING_t	 typeII_PortSelectionRI_Restriction;
							
							/* Context for parsing across buffer boundaries */
							asn_struct_ctx_t _asn_ctx;
						} *typeII_PortSelection;
					} choice;
					
					/* Context for parsing across buffer boundaries */
					asn_struct_ctx_t _asn_ctx;
				} subType;
				long	 phaseAlphabetSize;
				BOOLEAN_t	 subbandAmplitude;
				long	 numberOfBeams;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *type2;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} codebookType;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CodebookConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_portSelectionSamplingSize_57;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_phaseAlphabetSize_63;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_numberOfBeams_67;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_CodebookConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_CodebookConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_CodebookConfig_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _CodebookConfig_H_ */
#include <asn_internal.h>
