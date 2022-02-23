/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "FilterCoefficient.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_FilterCoefficient_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_FilterCoefficient_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  4,  4,  0,  15 }	/* (0..15,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_FilterCoefficient_value2enum_1[] = {
	{ 0,	3,	"fc0" },
	{ 1,	3,	"fc1" },
	{ 2,	3,	"fc2" },
	{ 3,	3,	"fc3" },
	{ 4,	3,	"fc4" },
	{ 5,	3,	"fc5" },
	{ 6,	3,	"fc6" },
	{ 7,	3,	"fc7" },
	{ 8,	3,	"fc8" },
	{ 9,	3,	"fc9" },
	{ 10,	4,	"fc11" },
	{ 11,	4,	"fc13" },
	{ 12,	4,	"fc15" },
	{ 13,	4,	"fc17" },
	{ 14,	4,	"fc19" },
	{ 15,	6,	"spare1" }
	/* This list is extensible */
};
static const unsigned int asn_MAP_FilterCoefficient_enum2value_1[] = {
	0,	/* fc0(0) */
	1,	/* fc1(1) */
	10,	/* fc11(10) */
	11,	/* fc13(11) */
	12,	/* fc15(12) */
	13,	/* fc17(13) */
	14,	/* fc19(14) */
	2,	/* fc2(2) */
	3,	/* fc3(3) */
	4,	/* fc4(4) */
	5,	/* fc5(5) */
	6,	/* fc6(6) */
	7,	/* fc7(7) */
	8,	/* fc8(8) */
	9,	/* fc9(9) */
	15	/* spare1(15) */
	/* This list is extensible */
};
const asn_INTEGER_specifics_t asn_SPC_FilterCoefficient_specs_1 = {
	asn_MAP_FilterCoefficient_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_FilterCoefficient_enum2value_1,	/* N => "tag"; sorted by N */
	16,	/* Number of elements in the maps */
	17,	/* Extensions before this member */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_FilterCoefficient_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_FilterCoefficient = {
	"FilterCoefficient",
	"FilterCoefficient",
	&asn_OP_NativeEnumerated,
	asn_DEF_FilterCoefficient_tags_1,
	sizeof(asn_DEF_FilterCoefficient_tags_1)
		/sizeof(asn_DEF_FilterCoefficient_tags_1[0]), /* 1 */
	asn_DEF_FilterCoefficient_tags_1,	/* Same as above */
	sizeof(asn_DEF_FilterCoefficient_tags_1)
		/sizeof(asn_DEF_FilterCoefficient_tags_1[0]), /* 1 */
	{ &asn_OER_type_FilterCoefficient_constr_1, &asn_PER_type_FilterCoefficient_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_FilterCoefficient_specs_1	/* Additional specs */
};

