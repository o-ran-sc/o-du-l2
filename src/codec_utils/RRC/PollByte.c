/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "PollByte.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_PollByte_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_PollByte_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 6,  6,  0,  63 }	/* (0..63) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_PollByte_value2enum_1[] = {
	{ 0,	3,	"kB1" },
	{ 1,	3,	"kB2" },
	{ 2,	3,	"kB5" },
	{ 3,	3,	"kB8" },
	{ 4,	4,	"kB10" },
	{ 5,	4,	"kB15" },
	{ 6,	4,	"kB25" },
	{ 7,	4,	"kB50" },
	{ 8,	4,	"kB75" },
	{ 9,	5,	"kB100" },
	{ 10,	5,	"kB125" },
	{ 11,	5,	"kB250" },
	{ 12,	5,	"kB375" },
	{ 13,	5,	"kB500" },
	{ 14,	5,	"kB750" },
	{ 15,	6,	"kB1000" },
	{ 16,	6,	"kB1250" },
	{ 17,	6,	"kB1500" },
	{ 18,	6,	"kB2000" },
	{ 19,	6,	"kB3000" },
	{ 20,	6,	"kB4000" },
	{ 21,	6,	"kB4500" },
	{ 22,	6,	"kB5000" },
	{ 23,	6,	"kB5500" },
	{ 24,	6,	"kB6000" },
	{ 25,	6,	"kB6500" },
	{ 26,	6,	"kB7000" },
	{ 27,	6,	"kB7500" },
	{ 28,	3,	"mB8" },
	{ 29,	3,	"mB9" },
	{ 30,	4,	"mB10" },
	{ 31,	4,	"mB11" },
	{ 32,	4,	"mB12" },
	{ 33,	4,	"mB13" },
	{ 34,	4,	"mB14" },
	{ 35,	4,	"mB15" },
	{ 36,	4,	"mB16" },
	{ 37,	4,	"mB17" },
	{ 38,	4,	"mB18" },
	{ 39,	4,	"mB20" },
	{ 40,	4,	"mB25" },
	{ 41,	4,	"mB30" },
	{ 42,	4,	"mB40" },
	{ 43,	8,	"infinity" },
	{ 44,	7,	"spare20" },
	{ 45,	7,	"spare19" },
	{ 46,	7,	"spare18" },
	{ 47,	7,	"spare17" },
	{ 48,	7,	"spare16" },
	{ 49,	7,	"spare15" },
	{ 50,	7,	"spare14" },
	{ 51,	7,	"spare13" },
	{ 52,	7,	"spare12" },
	{ 53,	7,	"spare11" },
	{ 54,	7,	"spare10" },
	{ 55,	6,	"spare9" },
	{ 56,	6,	"spare8" },
	{ 57,	6,	"spare7" },
	{ 58,	6,	"spare6" },
	{ 59,	6,	"spare5" },
	{ 60,	6,	"spare4" },
	{ 61,	6,	"spare3" },
	{ 62,	6,	"spare2" },
	{ 63,	6,	"spare1" }
};
static const unsigned int asn_MAP_PollByte_enum2value_1[] = {
	43,	/* infinity(43) */
	0,	/* kB1(0) */
	4,	/* kB10(4) */
	9,	/* kB100(9) */
	15,	/* kB1000(15) */
	10,	/* kB125(10) */
	16,	/* kB1250(16) */
	5,	/* kB15(5) */
	17,	/* kB1500(17) */
	1,	/* kB2(1) */
	18,	/* kB2000(18) */
	6,	/* kB25(6) */
	11,	/* kB250(11) */
	19,	/* kB3000(19) */
	12,	/* kB375(12) */
	20,	/* kB4000(20) */
	21,	/* kB4500(21) */
	2,	/* kB5(2) */
	7,	/* kB50(7) */
	13,	/* kB500(13) */
	22,	/* kB5000(22) */
	23,	/* kB5500(23) */
	24,	/* kB6000(24) */
	25,	/* kB6500(25) */
	26,	/* kB7000(26) */
	8,	/* kB75(8) */
	14,	/* kB750(14) */
	27,	/* kB7500(27) */
	3,	/* kB8(3) */
	30,	/* mB10(30) */
	31,	/* mB11(31) */
	32,	/* mB12(32) */
	33,	/* mB13(33) */
	34,	/* mB14(34) */
	35,	/* mB15(35) */
	36,	/* mB16(36) */
	37,	/* mB17(37) */
	38,	/* mB18(38) */
	39,	/* mB20(39) */
	40,	/* mB25(40) */
	41,	/* mB30(41) */
	42,	/* mB40(42) */
	28,	/* mB8(28) */
	29,	/* mB9(29) */
	63,	/* spare1(63) */
	54,	/* spare10(54) */
	53,	/* spare11(53) */
	52,	/* spare12(52) */
	51,	/* spare13(51) */
	50,	/* spare14(50) */
	49,	/* spare15(49) */
	48,	/* spare16(48) */
	47,	/* spare17(47) */
	46,	/* spare18(46) */
	45,	/* spare19(45) */
	62,	/* spare2(62) */
	44,	/* spare20(44) */
	61,	/* spare3(61) */
	60,	/* spare4(60) */
	59,	/* spare5(59) */
	58,	/* spare6(58) */
	57,	/* spare7(57) */
	56,	/* spare8(56) */
	55	/* spare9(55) */
};
const asn_INTEGER_specifics_t asn_SPC_PollByte_specs_1 = {
	asn_MAP_PollByte_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_PollByte_enum2value_1,	/* N => "tag"; sorted by N */
	64,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_PollByte_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_PollByte = {
	"PollByte",
	"PollByte",
	&asn_OP_NativeEnumerated,
	asn_DEF_PollByte_tags_1,
	sizeof(asn_DEF_PollByte_tags_1)
		/sizeof(asn_DEF_PollByte_tags_1[0]), /* 1 */
	asn_DEF_PollByte_tags_1,	/* Same as above */
	sizeof(asn_DEF_PollByte_tags_1)
		/sizeof(asn_DEF_PollByte_tags_1[0]), /* 1 */
	{ &asn_OER_type_PollByte_constr_1, &asn_PER_type_PollByte_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_PollByte_specs_1	/* Additional specs */
};

