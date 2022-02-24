/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "MeasAndMobParametersMRDC-FRX-Diff.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_simultaneousRxDataSSB_DiffNumerology_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_simultaneousRxDataSSB_DiffNumerology_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_simultaneousRxDataSSB_DiffNumerology_value2enum_2[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_simultaneousRxDataSSB_DiffNumerology_enum2value_2[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_simultaneousRxDataSSB_DiffNumerology_specs_2 = {
	asn_MAP_simultaneousRxDataSSB_DiffNumerology_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_simultaneousRxDataSSB_DiffNumerology_enum2value_2,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_simultaneousRxDataSSB_DiffNumerology_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_simultaneousRxDataSSB_DiffNumerology_2 = {
	"simultaneousRxDataSSB-DiffNumerology",
	"simultaneousRxDataSSB-DiffNumerology",
	&asn_OP_NativeEnumerated,
	asn_DEF_simultaneousRxDataSSB_DiffNumerology_tags_2,
	sizeof(asn_DEF_simultaneousRxDataSSB_DiffNumerology_tags_2)
		/sizeof(asn_DEF_simultaneousRxDataSSB_DiffNumerology_tags_2[0]) - 1, /* 1 */
	asn_DEF_simultaneousRxDataSSB_DiffNumerology_tags_2,	/* Same as above */
	sizeof(asn_DEF_simultaneousRxDataSSB_DiffNumerology_tags_2)
		/sizeof(asn_DEF_simultaneousRxDataSSB_DiffNumerology_tags_2[0]), /* 2 */
	{ &asn_OER_type_simultaneousRxDataSSB_DiffNumerology_constr_2, &asn_PER_type_simultaneousRxDataSSB_DiffNumerology_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_simultaneousRxDataSSB_DiffNumerology_specs_2	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_MeasAndMobParametersMRDC_FRX_Diff_1[] = {
	{ ATF_POINTER, 1, offsetof(struct MeasAndMobParametersMRDC_FRX_Diff, simultaneousRxDataSSB_DiffNumerology),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_simultaneousRxDataSSB_DiffNumerology_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"simultaneousRxDataSSB-DiffNumerology"
		},
};
static const int asn_MAP_MeasAndMobParametersMRDC_FRX_Diff_oms_1[] = { 0 };
static const ber_tlv_tag_t asn_DEF_MeasAndMobParametersMRDC_FRX_Diff_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_MeasAndMobParametersMRDC_FRX_Diff_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* simultaneousRxDataSSB-DiffNumerology */
};
asn_SEQUENCE_specifics_t asn_SPC_MeasAndMobParametersMRDC_FRX_Diff_specs_1 = {
	sizeof(struct MeasAndMobParametersMRDC_FRX_Diff),
	offsetof(struct MeasAndMobParametersMRDC_FRX_Diff, _asn_ctx),
	asn_MAP_MeasAndMobParametersMRDC_FRX_Diff_tag2el_1,
	1,	/* Count of tags in the map */
	asn_MAP_MeasAndMobParametersMRDC_FRX_Diff_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_MeasAndMobParametersMRDC_FRX_Diff = {
	"MeasAndMobParametersMRDC-FRX-Diff",
	"MeasAndMobParametersMRDC-FRX-Diff",
	&asn_OP_SEQUENCE,
	asn_DEF_MeasAndMobParametersMRDC_FRX_Diff_tags_1,
	sizeof(asn_DEF_MeasAndMobParametersMRDC_FRX_Diff_tags_1)
		/sizeof(asn_DEF_MeasAndMobParametersMRDC_FRX_Diff_tags_1[0]), /* 1 */
	asn_DEF_MeasAndMobParametersMRDC_FRX_Diff_tags_1,	/* Same as above */
	sizeof(asn_DEF_MeasAndMobParametersMRDC_FRX_Diff_tags_1)
		/sizeof(asn_DEF_MeasAndMobParametersMRDC_FRX_Diff_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_MeasAndMobParametersMRDC_FRX_Diff_1,
	1,	/* Elements count */
	&asn_SPC_MeasAndMobParametersMRDC_FRX_Diff_specs_1	/* Additional specs */
};

