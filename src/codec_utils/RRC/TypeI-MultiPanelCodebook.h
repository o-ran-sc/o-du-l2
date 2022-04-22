/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_TypeI_MultiPanelCodebook_H_
#define	_TypeI_MultiPanelCodebook_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum TypeI_MultiPanelCodebook__maxNumberTxPortsPerResource {
	TypeI_MultiPanelCodebook__maxNumberTxPortsPerResource_p8	= 0,
	TypeI_MultiPanelCodebook__maxNumberTxPortsPerResource_p16	= 1,
	TypeI_MultiPanelCodebook__maxNumberTxPortsPerResource_p32	= 2
} e_TypeI_MultiPanelCodebook__maxNumberTxPortsPerResource;
typedef enum TypeI_MultiPanelCodebook__supportedCodebookMode {
	TypeI_MultiPanelCodebook__supportedCodebookMode_mode1	= 0,
	TypeI_MultiPanelCodebook__supportedCodebookMode_mode2	= 1,
	TypeI_MultiPanelCodebook__supportedCodebookMode_both	= 2
} e_TypeI_MultiPanelCodebook__supportedCodebookMode;
typedef enum TypeI_MultiPanelCodebook__supportedNumberPanels {
	TypeI_MultiPanelCodebook__supportedNumberPanels_n2	= 0,
	TypeI_MultiPanelCodebook__supportedNumberPanels_n4	= 1
} e_TypeI_MultiPanelCodebook__supportedNumberPanels;

/* TypeI-MultiPanelCodebook */
typedef struct TypeI_MultiPanelCodebook {
	long	 maxNumberTxPortsPerResource;
	long	 maxNumberResources;
	long	 totalNumberTxPorts;
	long	 supportedCodebookMode;
	long	 supportedNumberPanels;
	long	 maxNumberCSI_RS_PerResourceSet;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TypeI_MultiPanelCodebook_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_maxNumberTxPortsPerResource_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_supportedCodebookMode_8;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_supportedNumberPanels_12;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_TypeI_MultiPanelCodebook;
extern asn_SEQUENCE_specifics_t asn_SPC_TypeI_MultiPanelCodebook_specs_1;
extern asn_TYPE_member_t asn_MBR_TypeI_MultiPanelCodebook_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _TypeI_MultiPanelCodebook_H_ */
#include <asn_internal.h>
