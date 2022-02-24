/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "PollPDU.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_PollPDU_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_PollPDU_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 5,  5,  0,  31 }	/* (0..31) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_PollPDU_value2enum_1[] = {
	{ 0,	2,	"p4" },
	{ 1,	2,	"p8" },
	{ 2,	3,	"p16" },
	{ 3,	3,	"p32" },
	{ 4,	3,	"p64" },
	{ 5,	4,	"p128" },
	{ 6,	4,	"p256" },
	{ 7,	4,	"p512" },
	{ 8,	5,	"p1024" },
	{ 9,	5,	"p2048" },
	{ 10,	5,	"p4096" },
	{ 11,	5,	"p6144" },
	{ 12,	5,	"p8192" },
	{ 13,	6,	"p12288" },
	{ 14,	6,	"p16384" },
	{ 15,	6,	"p20480" },
	{ 16,	6,	"p24576" },
	{ 17,	6,	"p28672" },
	{ 18,	6,	"p32768" },
	{ 19,	6,	"p40960" },
	{ 20,	6,	"p49152" },
	{ 21,	6,	"p57344" },
	{ 22,	6,	"p65536" },
	{ 23,	8,	"infinity" },
	{ 24,	6,	"spare8" },
	{ 25,	6,	"spare7" },
	{ 26,	6,	"spare6" },
	{ 27,	6,	"spare5" },
	{ 28,	6,	"spare4" },
	{ 29,	6,	"spare3" },
	{ 30,	6,	"spare2" },
	{ 31,	6,	"spare1" }
};
static const unsigned int asn_MAP_PollPDU_enum2value_1[] = {
	23,	/* infinity(23) */
	8,	/* p1024(8) */
	13,	/* p12288(13) */
	5,	/* p128(5) */
	2,	/* p16(2) */
	14,	/* p16384(14) */
	9,	/* p2048(9) */
	15,	/* p20480(15) */
	16,	/* p24576(16) */
	6,	/* p256(6) */
	17,	/* p28672(17) */
	3,	/* p32(3) */
	18,	/* p32768(18) */
	0,	/* p4(0) */
	10,	/* p4096(10) */
	19,	/* p40960(19) */
	20,	/* p49152(20) */
	7,	/* p512(7) */
	21,	/* p57344(21) */
	11,	/* p6144(11) */
	4,	/* p64(4) */
	22,	/* p65536(22) */
	1,	/* p8(1) */
	12,	/* p8192(12) */
	31,	/* spare1(31) */
	30,	/* spare2(30) */
	29,	/* spare3(29) */
	28,	/* spare4(28) */
	27,	/* spare5(27) */
	26,	/* spare6(26) */
	25,	/* spare7(25) */
	24	/* spare8(24) */
};
const asn_INTEGER_specifics_t asn_SPC_PollPDU_specs_1 = {
	asn_MAP_PollPDU_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_PollPDU_enum2value_1,	/* N => "tag"; sorted by N */
	32,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_PollPDU_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_PollPDU = {
	"PollPDU",
	"PollPDU",
	&asn_OP_NativeEnumerated,
	asn_DEF_PollPDU_tags_1,
	sizeof(asn_DEF_PollPDU_tags_1)
		/sizeof(asn_DEF_PollPDU_tags_1[0]), /* 1 */
	asn_DEF_PollPDU_tags_1,	/* Same as above */
	sizeof(asn_DEF_PollPDU_tags_1)
		/sizeof(asn_DEF_PollPDU_tags_1[0]), /* 1 */
	{ &asn_OER_type_PollPDU_constr_1, &asn_PER_type_PollPDU_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_PollPDU_specs_1	/* Additional specs */
};

