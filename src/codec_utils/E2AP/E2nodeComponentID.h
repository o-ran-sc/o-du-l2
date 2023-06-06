/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_E2nodeComponentID_H_
#define	_E2nodeComponentID_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum E2nodeComponentID_PR {
	E2nodeComponentID_PR_NOTHING,	/* No components present */
	E2nodeComponentID_PR_e2nodeComponentInterfaceTypeNG,
	E2nodeComponentID_PR_e2nodeComponentInterfaceTypeXn,
	E2nodeComponentID_PR_e2nodeComponentInterfaceTypeE1,
	E2nodeComponentID_PR_e2nodeComponentInterfaceTypeF1,
	E2nodeComponentID_PR_e2nodeComponentInterfaceTypeW1,
	E2nodeComponentID_PR_e2nodeComponentInterfaceTypeS1,
	E2nodeComponentID_PR_e2nodeComponentInterfaceTypeX2
	/* Extensions may appear below */
	
} E2nodeComponentID_PR;

/* Forward declarations */
struct E2nodeComponentInterfaceNG;
struct E2nodeComponentInterfaceXn;
struct E2nodeComponentInterfaceE1;
struct E2nodeComponentInterfaceF1;
struct E2nodeComponentInterfaceW1;
struct E2nodeComponentInterfaceS1;
struct E2nodeComponentInterfaceX2;

/* E2nodeComponentID */
typedef struct E2nodeComponentID {
	E2nodeComponentID_PR present;
	union E2nodeComponentID_u {
		struct E2nodeComponentInterfaceNG	*e2nodeComponentInterfaceTypeNG;
		struct E2nodeComponentInterfaceXn	*e2nodeComponentInterfaceTypeXn;
		struct E2nodeComponentInterfaceE1	*e2nodeComponentInterfaceTypeE1;
		struct E2nodeComponentInterfaceF1	*e2nodeComponentInterfaceTypeF1;
		struct E2nodeComponentInterfaceW1	*e2nodeComponentInterfaceTypeW1;
		struct E2nodeComponentInterfaceS1	*e2nodeComponentInterfaceTypeS1;
		struct E2nodeComponentInterfaceX2	*e2nodeComponentInterfaceTypeX2;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} E2nodeComponentID_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_E2nodeComponentID;
extern asn_CHOICE_specifics_t asn_SPC_E2nodeComponentID_specs_1;
extern asn_TYPE_member_t asn_MBR_E2nodeComponentID_1[7];
extern asn_per_constraints_t asn_PER_type_E2nodeComponentID_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _E2nodeComponentID_H_ */
#include <asn_internal.h>
