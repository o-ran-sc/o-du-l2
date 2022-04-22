/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SN-FieldLengthAM.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_SN_FieldLengthAM_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_SN_FieldLengthAM_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_SN_FieldLengthAM_value2enum_1[] = {
	{ 0,	6,	"size12" },
	{ 1,	6,	"size18" }
};
static const unsigned int asn_MAP_SN_FieldLengthAM_enum2value_1[] = {
	0,	/* size12(0) */
	1	/* size18(1) */
};
const asn_INTEGER_specifics_t asn_SPC_SN_FieldLengthAM_specs_1 = {
	asn_MAP_SN_FieldLengthAM_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_SN_FieldLengthAM_enum2value_1,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_SN_FieldLengthAM_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_SN_FieldLengthAM = {
	"SN-FieldLengthAM",
	"SN-FieldLengthAM",
	&asn_OP_NativeEnumerated,
	asn_DEF_SN_FieldLengthAM_tags_1,
	sizeof(asn_DEF_SN_FieldLengthAM_tags_1)
		/sizeof(asn_DEF_SN_FieldLengthAM_tags_1[0]), /* 1 */
	asn_DEF_SN_FieldLengthAM_tags_1,	/* Same as above */
	sizeof(asn_DEF_SN_FieldLengthAM_tags_1)
		/sizeof(asn_DEF_SN_FieldLengthAM_tags_1[0]), /* 1 */
	{ &asn_OER_type_SN_FieldLengthAM_constr_1, &asn_PER_type_SN_FieldLengthAM_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_SN_FieldLengthAM_specs_1	/* Additional specs */
};

