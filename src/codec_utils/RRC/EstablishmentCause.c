/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "EstablishmentCause.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_EstablishmentCause_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_EstablishmentCause_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  15 }	/* (0..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_EstablishmentCause_value2enum_1[] = {
	{ 0,	9,	"emergency" },
	{ 1,	18,	"highPriorityAccess" },
	{ 2,	9,	"mt-Access" },
	{ 3,	13,	"mo-Signalling" },
	{ 4,	7,	"mo-Data" },
	{ 5,	12,	"mo-VoiceCall" },
	{ 6,	12,	"mo-VideoCall" },
	{ 7,	6,	"mo-SMS" },
	{ 8,	18,	"mps-PriorityAccess" },
	{ 9,	18,	"mcs-PriorityAccess" },
	{ 10,	6,	"spare6" },
	{ 11,	6,	"spare5" },
	{ 12,	6,	"spare4" },
	{ 13,	6,	"spare3" },
	{ 14,	6,	"spare2" },
	{ 15,	6,	"spare1" }
};
static const unsigned int asn_MAP_EstablishmentCause_enum2value_1[] = {
	0,	/* emergency(0) */
	1,	/* highPriorityAccess(1) */
	9,	/* mcs-PriorityAccess(9) */
	4,	/* mo-Data(4) */
	7,	/* mo-SMS(7) */
	3,	/* mo-Signalling(3) */
	6,	/* mo-VideoCall(6) */
	5,	/* mo-VoiceCall(5) */
	8,	/* mps-PriorityAccess(8) */
	2,	/* mt-Access(2) */
	15,	/* spare1(15) */
	14,	/* spare2(14) */
	13,	/* spare3(13) */
	12,	/* spare4(12) */
	11,	/* spare5(11) */
	10	/* spare6(10) */
};
const asn_INTEGER_specifics_t asn_SPC_EstablishmentCause_specs_1 = {
	asn_MAP_EstablishmentCause_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_EstablishmentCause_enum2value_1,	/* N => "tag"; sorted by N */
	16,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_EstablishmentCause_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_EstablishmentCause = {
	"EstablishmentCause",
	"EstablishmentCause",
	&asn_OP_NativeEnumerated,
	asn_DEF_EstablishmentCause_tags_1,
	sizeof(asn_DEF_EstablishmentCause_tags_1)
		/sizeof(asn_DEF_EstablishmentCause_tags_1[0]), /* 1 */
	asn_DEF_EstablishmentCause_tags_1,	/* Same as above */
	sizeof(asn_DEF_EstablishmentCause_tags_1)
		/sizeof(asn_DEF_EstablishmentCause_tags_1[0]), /* 1 */
	{ &asn_OER_type_EstablishmentCause_constr_1, &asn_PER_type_EstablishmentCause_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_EstablishmentCause_specs_1	/* Additional specs */
};

