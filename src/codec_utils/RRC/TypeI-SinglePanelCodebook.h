/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_TypeI_SinglePanelCodebook_H_
#define	_TypeI_SinglePanelCodebook_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum TypeI_SinglePanelCodebook__maxNumberTxPortsPerResource {
	TypeI_SinglePanelCodebook__maxNumberTxPortsPerResource_p2	= 0,
	TypeI_SinglePanelCodebook__maxNumberTxPortsPerResource_p4	= 1,
	TypeI_SinglePanelCodebook__maxNumberTxPortsPerResource_p8	= 2,
	TypeI_SinglePanelCodebook__maxNumberTxPortsPerResource_p12	= 3,
	TypeI_SinglePanelCodebook__maxNumberTxPortsPerResource_p16	= 4,
	TypeI_SinglePanelCodebook__maxNumberTxPortsPerResource_p24	= 5,
	TypeI_SinglePanelCodebook__maxNumberTxPortsPerResource_p32	= 6
} e_TypeI_SinglePanelCodebook__maxNumberTxPortsPerResource;
typedef enum TypeI_SinglePanelCodebook__supportedCodebookMode {
	TypeI_SinglePanelCodebook__supportedCodebookMode_mode1	= 0,
	TypeI_SinglePanelCodebook__supportedCodebookMode_mode1AndMode2	= 1
} e_TypeI_SinglePanelCodebook__supportedCodebookMode;

/* TypeI-SinglePanelCodebook */
typedef struct TypeI_SinglePanelCodebook {
	long	 maxNumberTxPortsPerResource;
	long	 maxNumberResources;
	long	 totalNumberTxPorts;
	long	 supportedCodebookMode;
	long	 maxNumberCSI_RS_PerResourceSet;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TypeI_SinglePanelCodebook_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_maxNumberTxPortsPerResource_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_supportedCodebookMode_12;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_TypeI_SinglePanelCodebook;
extern asn_SEQUENCE_specifics_t asn_SPC_TypeI_SinglePanelCodebook_specs_1;
extern asn_TYPE_member_t asn_MBR_TypeI_SinglePanelCodebook_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _TypeI_SinglePanelCodebook_H_ */
#include <asn_internal.h>
