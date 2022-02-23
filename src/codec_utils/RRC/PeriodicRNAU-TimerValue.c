/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "PeriodicRNAU-TimerValue.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_PeriodicRNAU_TimerValue_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_PeriodicRNAU_TimerValue_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_PeriodicRNAU_TimerValue_value2enum_1[] = {
	{ 0,	4,	"min5" },
	{ 1,	5,	"min10" },
	{ 2,	5,	"min20" },
	{ 3,	5,	"min30" },
	{ 4,	5,	"min60" },
	{ 5,	6,	"min120" },
	{ 6,	6,	"min360" },
	{ 7,	6,	"min720" }
};
static const unsigned int asn_MAP_PeriodicRNAU_TimerValue_enum2value_1[] = {
	1,	/* min10(1) */
	5,	/* min120(5) */
	2,	/* min20(2) */
	3,	/* min30(3) */
	6,	/* min360(6) */
	0,	/* min5(0) */
	4,	/* min60(4) */
	7	/* min720(7) */
};
const asn_INTEGER_specifics_t asn_SPC_PeriodicRNAU_TimerValue_specs_1 = {
	asn_MAP_PeriodicRNAU_TimerValue_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_PeriodicRNAU_TimerValue_enum2value_1,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_PeriodicRNAU_TimerValue_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_PeriodicRNAU_TimerValue = {
	"PeriodicRNAU-TimerValue",
	"PeriodicRNAU-TimerValue",
	&asn_OP_NativeEnumerated,
	asn_DEF_PeriodicRNAU_TimerValue_tags_1,
	sizeof(asn_DEF_PeriodicRNAU_TimerValue_tags_1)
		/sizeof(asn_DEF_PeriodicRNAU_TimerValue_tags_1[0]), /* 1 */
	asn_DEF_PeriodicRNAU_TimerValue_tags_1,	/* Same as above */
	sizeof(asn_DEF_PeriodicRNAU_TimerValue_tags_1)
		/sizeof(asn_DEF_PeriodicRNAU_TimerValue_tags_1[0]), /* 1 */
	{ &asn_OER_type_PeriodicRNAU_TimerValue_constr_1, &asn_PER_type_PeriodicRNAU_TimerValue_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_PeriodicRNAU_TimerValue_specs_1	/* Additional specs */
};

