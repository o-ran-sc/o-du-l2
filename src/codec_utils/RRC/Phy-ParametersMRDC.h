/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_Phy_ParametersMRDC_H_
#define	_Phy_ParametersMRDC_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct NAICS_Capability_Entry;

/* Phy-ParametersMRDC */
typedef struct Phy_ParametersMRDC {
	struct Phy_ParametersMRDC__naics_Capability_List {
		A_SEQUENCE_OF(struct NAICS_Capability_Entry) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *naics_Capability_List;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Phy_ParametersMRDC_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Phy_ParametersMRDC;
extern asn_SEQUENCE_specifics_t asn_SPC_Phy_ParametersMRDC_specs_1;
extern asn_TYPE_member_t asn_MBR_Phy_ParametersMRDC_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _Phy_ParametersMRDC_H_ */
#include <asn_internal.h>
