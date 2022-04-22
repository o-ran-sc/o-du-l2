/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MAC_Parameters_H_
#define	_MAC_Parameters_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct MAC_ParametersCommon;
struct MAC_ParametersXDD_Diff;

/* MAC-Parameters */
typedef struct MAC_Parameters {
	struct MAC_ParametersCommon	*mac_ParametersCommon;	/* OPTIONAL */
	struct MAC_ParametersXDD_Diff	*mac_ParametersXDD_Diff;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MAC_Parameters_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MAC_Parameters;
extern asn_SEQUENCE_specifics_t asn_SPC_MAC_Parameters_specs_1;
extern asn_TYPE_member_t asn_MBR_MAC_Parameters_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _MAC_Parameters_H_ */
#include <asn_internal.h>
