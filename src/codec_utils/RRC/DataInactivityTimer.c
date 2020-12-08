/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "DataInactivityTimer.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_DataInactivityTimer_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_DataInactivityTimer_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  15 }	/* (0..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_DataInactivityTimer_value2enum_1[] = {
	{ 0,	2,	"s1" },
	{ 1,	2,	"s2" },
	{ 2,	2,	"s3" },
	{ 3,	2,	"s5" },
	{ 4,	2,	"s7" },
	{ 5,	3,	"s10" },
	{ 6,	3,	"s15" },
	{ 7,	3,	"s20" },
	{ 8,	3,	"s40" },
	{ 9,	3,	"s50" },
	{ 10,	3,	"s60" },
	{ 11,	3,	"s80" },
	{ 12,	4,	"s100" },
	{ 13,	4,	"s120" },
	{ 14,	4,	"s150" },
	{ 15,	4,	"s180" }
};
static const unsigned int asn_MAP_DataInactivityTimer_enum2value_1[] = {
	0,	/* s1(0) */
	5,	/* s10(5) */
	12,	/* s100(12) */
	13,	/* s120(13) */
	6,	/* s15(6) */
	14,	/* s150(14) */
	15,	/* s180(15) */
	1,	/* s2(1) */
	7,	/* s20(7) */
	2,	/* s3(2) */
	8,	/* s40(8) */
	3,	/* s5(3) */
	9,	/* s50(9) */
	10,	/* s60(10) */
	4,	/* s7(4) */
	11	/* s80(11) */
};
const asn_INTEGER_specifics_t asn_SPC_DataInactivityTimer_specs_1 = {
	asn_MAP_DataInactivityTimer_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_DataInactivityTimer_enum2value_1,	/* N => "tag"; sorted by N */
	16,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_DataInactivityTimer_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_DataInactivityTimer = {
	"DataInactivityTimer",
	"DataInactivityTimer",
	&asn_OP_NativeEnumerated,
	asn_DEF_DataInactivityTimer_tags_1,
	sizeof(asn_DEF_DataInactivityTimer_tags_1)
		/sizeof(asn_DEF_DataInactivityTimer_tags_1[0]), /* 1 */
	asn_DEF_DataInactivityTimer_tags_1,	/* Same as above */
	sizeof(asn_DEF_DataInactivityTimer_tags_1)
		/sizeof(asn_DEF_DataInactivityTimer_tags_1[0]), /* 1 */
	{ &asn_OER_type_DataInactivityTimer_constr_1, &asn_PER_type_DataInactivityTimer_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_DataInactivityTimer_specs_1	/* Additional specs */
};

