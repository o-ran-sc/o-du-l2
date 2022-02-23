/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_TypeII_CodebookPortSelection_H_
#define	_TypeII_CodebookPortSelection_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum TypeII_CodebookPortSelection__maxNumberTxPortsPerResource {
	TypeII_CodebookPortSelection__maxNumberTxPortsPerResource_p4	= 0,
	TypeII_CodebookPortSelection__maxNumberTxPortsPerResource_p8	= 1,
	TypeII_CodebookPortSelection__maxNumberTxPortsPerResource_p12	= 2,
	TypeII_CodebookPortSelection__maxNumberTxPortsPerResource_p16	= 3,
	TypeII_CodebookPortSelection__maxNumberTxPortsPerResource_p24	= 4,
	TypeII_CodebookPortSelection__maxNumberTxPortsPerResource_p32	= 5
} e_TypeII_CodebookPortSelection__maxNumberTxPortsPerResource;
typedef enum TypeII_CodebookPortSelection__amplitudeScalingType {
	TypeII_CodebookPortSelection__amplitudeScalingType_wideband	= 0,
	TypeII_CodebookPortSelection__amplitudeScalingType_widebandAndSubband	= 1
} e_TypeII_CodebookPortSelection__amplitudeScalingType;

/* TypeII-CodebookPortSelection */
typedef struct TypeII_CodebookPortSelection {
	long	 maxNumberTxPortsPerResource;
	long	 maxNumberResources;
	long	 totalNumberTxPorts;
	long	 parameterLx;
	long	 amplitudeScalingType;
	long	 maxNumberCSI_RS_PerResourceSet;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TypeII_CodebookPortSelection_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_maxNumberTxPortsPerResource_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_amplitudeScalingType_12;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_TypeII_CodebookPortSelection;
extern asn_SEQUENCE_specifics_t asn_SPC_TypeII_CodebookPortSelection_specs_1;
extern asn_TYPE_member_t asn_MBR_TypeII_CodebookPortSelection_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _TypeII_CodebookPortSelection_H_ */
#include <asn_internal.h>
