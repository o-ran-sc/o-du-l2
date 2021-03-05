/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "codeathon/E2AP.asn1"
 * 	`asn1c -D ./newE2AP -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "CauseRIC.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_CauseRIC_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_CauseRIC_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  4,  4,  0,  10 }	/* (0..10,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_CauseRIC_value2enum_1[] = {
	{ 0,	23,	"ran-function-id-Invalid" },
	{ 1,	20,	"action-not-supported" },
	{ 2,	17,	"excessive-actions" },
	{ 3,	16,	"duplicate-action" },
	{ 4,	15,	"duplicate-event" },
	{ 5,	23,	"function-resource-limit" },
	{ 6,	18,	"request-id-unknown" },
	{ 7,	46,	"inconsistent-action-subsequent-action-sequence" },
	{ 8,	23,	"control-message-invalid" },
	{ 9,	23,	"call-process-id-invalid" },
	{ 10,	11,	"unspecified" }
	/* This list is extensible */
};
static const unsigned int asn_MAP_CauseRIC_enum2value_1[] = {
	1,	/* action-not-supported(1) */
	9,	/* call-process-id-invalid(9) */
	8,	/* control-message-invalid(8) */
	3,	/* duplicate-action(3) */
	4,	/* duplicate-event(4) */
	2,	/* excessive-actions(2) */
	5,	/* function-resource-limit(5) */
	7,	/* inconsistent-action-subsequent-action-sequence(7) */
	0,	/* ran-function-id-Invalid(0) */
	6,	/* request-id-unknown(6) */
	10	/* unspecified(10) */
	/* This list is extensible */
};
const asn_INTEGER_specifics_t asn_SPC_CauseRIC_specs_1 = {
	asn_MAP_CauseRIC_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_CauseRIC_enum2value_1,	/* N => "tag"; sorted by N */
	11,	/* Number of elements in the maps */
	12,	/* Extensions before this member */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_CauseRIC_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_CauseRIC = {
	"CauseRIC",
	"CauseRIC",
	&asn_OP_NativeEnumerated,
	asn_DEF_CauseRIC_tags_1,
	sizeof(asn_DEF_CauseRIC_tags_1)
		/sizeof(asn_DEF_CauseRIC_tags_1[0]), /* 1 */
	asn_DEF_CauseRIC_tags_1,	/* Same as above */
	sizeof(asn_DEF_CauseRIC_tags_1)
		/sizeof(asn_DEF_CauseRIC_tags_1[0]), /* 1 */
	{ &asn_OER_type_CauseRIC_constr_1, &asn_PER_type_CauseRIC_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_CauseRIC_specs_1	/* Additional specs */
};

