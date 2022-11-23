/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "CA-BandwidthClassNR.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_CA_BandwidthClassNR_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_CA_BandwidthClassNR_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  5,  5,  0,  16 }	/* (0..16,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_CA_BandwidthClassNR_value2enum_1[] = {
	{ 0,	1,	"a" },
	{ 1,	1,	"b" },
	{ 2,	1,	"c" },
	{ 3,	1,	"d" },
	{ 4,	1,	"e" },
	{ 5,	1,	"f" },
	{ 6,	1,	"g" },
	{ 7,	1,	"h" },
	{ 8,	1,	"i" },
	{ 9,	1,	"j" },
	{ 10,	1,	"k" },
	{ 11,	1,	"l" },
	{ 12,	1,	"m" },
	{ 13,	1,	"n" },
	{ 14,	1,	"o" },
	{ 15,	1,	"p" },
	{ 16,	1,	"q" }
	/* This list is extensible */
};
static const unsigned int asn_MAP_CA_BandwidthClassNR_enum2value_1[] = {
	0,	/* a(0) */
	1,	/* b(1) */
	2,	/* c(2) */
	3,	/* d(3) */
	4,	/* e(4) */
	5,	/* f(5) */
	6,	/* g(6) */
	7,	/* h(7) */
	8,	/* i(8) */
	9,	/* j(9) */
	10,	/* k(10) */
	11,	/* l(11) */
	12,	/* m(12) */
	13,	/* n(13) */
	14,	/* o(14) */
	15,	/* p(15) */
	16	/* q(16) */
	/* This list is extensible */
};
const asn_INTEGER_specifics_t asn_SPC_CA_BandwidthClassNR_specs_1 = {
	asn_MAP_CA_BandwidthClassNR_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_CA_BandwidthClassNR_enum2value_1,	/* N => "tag"; sorted by N */
	17,	/* Number of elements in the maps */
	18,	/* Extensions before this member */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_CA_BandwidthClassNR_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_CA_BandwidthClassNR = {
	"CA-BandwidthClassNR",
	"CA-BandwidthClassNR",
	&asn_OP_NativeEnumerated,
	asn_DEF_CA_BandwidthClassNR_tags_1,
	sizeof(asn_DEF_CA_BandwidthClassNR_tags_1)
		/sizeof(asn_DEF_CA_BandwidthClassNR_tags_1[0]), /* 1 */
	asn_DEF_CA_BandwidthClassNR_tags_1,	/* Same as above */
	sizeof(asn_DEF_CA_BandwidthClassNR_tags_1)
		/sizeof(asn_DEF_CA_BandwidthClassNR_tags_1[0]), /* 1 */
	{ &asn_OER_type_CA_BandwidthClassNR_constr_1, &asn_PER_type_CA_BandwidthClassNR_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_CA_BandwidthClassNR_specs_1	/* Additional specs */
};

