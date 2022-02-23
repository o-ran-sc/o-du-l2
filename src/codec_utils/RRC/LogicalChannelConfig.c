/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "LogicalChannelConfig.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_priority_constraint_2(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 16)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_allowedServingCells_constraint_2(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	/* Determine the number of elements */
	size = _A_CSEQUENCE_FROM_VOID(sptr)->count;
	
	if((size >= 1 && size <= 31)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_allowedSCS_List_constraint_2(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	/* Determine the number of elements */
	size = _A_CSEQUENCE_FROM_VOID(sptr)->count;
	
	if((size >= 1 && size <= 5)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_logicalChannelGroup_constraint_2(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 7)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_prioritisedBitRate_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_prioritisedBitRate_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  15 }	/* (0..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_bucketSizeDuration_constr_21 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_bucketSizeDuration_constr_21 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  15 }	/* (0..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_allowedServingCells_constr_38 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..31)) */};
static asn_per_constraints_t asn_PER_type_allowedServingCells_constr_38 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  31 }	/* (SIZE(1..31)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_allowedSCS_List_constr_40 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..5)) */};
static asn_per_constraints_t asn_PER_type_allowedSCS_List_constr_40 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 3,  3,  1,  5 }	/* (SIZE(1..5)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_maxPUSCH_Duration_constr_42 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_maxPUSCH_Duration_constr_42 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_configuredGrantType1Allowed_constr_51 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_configuredGrantType1Allowed_constr_51 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_bitRateQueryProhibitTimer_constr_58 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_bitRateQueryProhibitTimer_constr_58 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_priority_constr_3 CC_NOTUSED = {
	{ 1, 1 }	/* (1..16) */,
	-1};
static asn_per_constraints_t asn_PER_memb_priority_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  1,  16 }	/* (1..16) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_allowedServingCells_constr_38 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..31)) */};
static asn_per_constraints_t asn_PER_memb_allowedServingCells_constr_38 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  31 }	/* (SIZE(1..31)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_allowedSCS_List_constr_40 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..5)) */};
static asn_per_constraints_t asn_PER_memb_allowedSCS_List_constr_40 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 3,  3,  1,  5 }	/* (SIZE(1..5)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_logicalChannelGroup_constr_53 CC_NOTUSED = {
	{ 1, 1 }	/* (0..7) */,
	-1};
static asn_per_constraints_t asn_PER_memb_logicalChannelGroup_constr_53 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_prioritisedBitRate_value2enum_4[] = {
	{ 0,	5,	"kBps0" },
	{ 1,	5,	"kBps8" },
	{ 2,	6,	"kBps16" },
	{ 3,	6,	"kBps32" },
	{ 4,	6,	"kBps64" },
	{ 5,	7,	"kBps128" },
	{ 6,	7,	"kBps256" },
	{ 7,	7,	"kBps512" },
	{ 8,	8,	"kBps1024" },
	{ 9,	8,	"kBps2048" },
	{ 10,	8,	"kBps4096" },
	{ 11,	8,	"kBps8192" },
	{ 12,	9,	"kBps16384" },
	{ 13,	9,	"kBps32768" },
	{ 14,	9,	"kBps65536" },
	{ 15,	8,	"infinity" }
};
static const unsigned int asn_MAP_prioritisedBitRate_enum2value_4[] = {
	15,	/* infinity(15) */
	0,	/* kBps0(0) */
	8,	/* kBps1024(8) */
	5,	/* kBps128(5) */
	2,	/* kBps16(2) */
	12,	/* kBps16384(12) */
	9,	/* kBps2048(9) */
	6,	/* kBps256(6) */
	3,	/* kBps32(3) */
	13,	/* kBps32768(13) */
	10,	/* kBps4096(10) */
	7,	/* kBps512(7) */
	4,	/* kBps64(4) */
	14,	/* kBps65536(14) */
	1,	/* kBps8(1) */
	11	/* kBps8192(11) */
};
static const asn_INTEGER_specifics_t asn_SPC_prioritisedBitRate_specs_4 = {
	asn_MAP_prioritisedBitRate_value2enum_4,	/* "tag" => N; sorted by tag */
	asn_MAP_prioritisedBitRate_enum2value_4,	/* N => "tag"; sorted by N */
	16,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_prioritisedBitRate_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_prioritisedBitRate_4 = {
	"prioritisedBitRate",
	"prioritisedBitRate",
	&asn_OP_NativeEnumerated,
	asn_DEF_prioritisedBitRate_tags_4,
	sizeof(asn_DEF_prioritisedBitRate_tags_4)
		/sizeof(asn_DEF_prioritisedBitRate_tags_4[0]) - 1, /* 1 */
	asn_DEF_prioritisedBitRate_tags_4,	/* Same as above */
	sizeof(asn_DEF_prioritisedBitRate_tags_4)
		/sizeof(asn_DEF_prioritisedBitRate_tags_4[0]), /* 2 */
	{ &asn_OER_type_prioritisedBitRate_constr_4, &asn_PER_type_prioritisedBitRate_constr_4, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_prioritisedBitRate_specs_4	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_bucketSizeDuration_value2enum_21[] = {
	{ 0,	3,	"ms5" },
	{ 1,	4,	"ms10" },
	{ 2,	4,	"ms20" },
	{ 3,	4,	"ms50" },
	{ 4,	5,	"ms100" },
	{ 5,	5,	"ms150" },
	{ 6,	5,	"ms300" },
	{ 7,	5,	"ms500" },
	{ 8,	6,	"ms1000" },
	{ 9,	6,	"spare7" },
	{ 10,	6,	"spare6" },
	{ 11,	6,	"spare5" },
	{ 12,	6,	"spare4" },
	{ 13,	6,	"spare3" },
	{ 14,	6,	"spare2" },
	{ 15,	6,	"spare1" }
};
static const unsigned int asn_MAP_bucketSizeDuration_enum2value_21[] = {
	1,	/* ms10(1) */
	4,	/* ms100(4) */
	8,	/* ms1000(8) */
	5,	/* ms150(5) */
	2,	/* ms20(2) */
	6,	/* ms300(6) */
	0,	/* ms5(0) */
	3,	/* ms50(3) */
	7,	/* ms500(7) */
	15,	/* spare1(15) */
	14,	/* spare2(14) */
	13,	/* spare3(13) */
	12,	/* spare4(12) */
	11,	/* spare5(11) */
	10,	/* spare6(10) */
	9	/* spare7(9) */
};
static const asn_INTEGER_specifics_t asn_SPC_bucketSizeDuration_specs_21 = {
	asn_MAP_bucketSizeDuration_value2enum_21,	/* "tag" => N; sorted by tag */
	asn_MAP_bucketSizeDuration_enum2value_21,	/* N => "tag"; sorted by N */
	16,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_bucketSizeDuration_tags_21[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_bucketSizeDuration_21 = {
	"bucketSizeDuration",
	"bucketSizeDuration",
	&asn_OP_NativeEnumerated,
	asn_DEF_bucketSizeDuration_tags_21,
	sizeof(asn_DEF_bucketSizeDuration_tags_21)
		/sizeof(asn_DEF_bucketSizeDuration_tags_21[0]) - 1, /* 1 */
	asn_DEF_bucketSizeDuration_tags_21,	/* Same as above */
	sizeof(asn_DEF_bucketSizeDuration_tags_21)
		/sizeof(asn_DEF_bucketSizeDuration_tags_21[0]), /* 2 */
	{ &asn_OER_type_bucketSizeDuration_constr_21, &asn_PER_type_bucketSizeDuration_constr_21, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_bucketSizeDuration_specs_21	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_allowedServingCells_38[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_ServCellIndex,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_allowedServingCells_tags_38[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_allowedServingCells_specs_38 = {
	sizeof(struct LogicalChannelConfig__ul_SpecificParameters__allowedServingCells),
	offsetof(struct LogicalChannelConfig__ul_SpecificParameters__allowedServingCells, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_allowedServingCells_38 = {
	"allowedServingCells",
	"allowedServingCells",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_allowedServingCells_tags_38,
	sizeof(asn_DEF_allowedServingCells_tags_38)
		/sizeof(asn_DEF_allowedServingCells_tags_38[0]) - 1, /* 1 */
	asn_DEF_allowedServingCells_tags_38,	/* Same as above */
	sizeof(asn_DEF_allowedServingCells_tags_38)
		/sizeof(asn_DEF_allowedServingCells_tags_38[0]), /* 2 */
	{ &asn_OER_type_allowedServingCells_constr_38, &asn_PER_type_allowedServingCells_constr_38, SEQUENCE_OF_constraint },
	asn_MBR_allowedServingCells_38,
	1,	/* Single element */
	&asn_SPC_allowedServingCells_specs_38	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_allowedSCS_List_40[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (10 << 2)),
		0,
		&asn_DEF_SubcarrierSpacing,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_allowedSCS_List_tags_40[] = {
	(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_allowedSCS_List_specs_40 = {
	sizeof(struct LogicalChannelConfig__ul_SpecificParameters__allowedSCS_List),
	offsetof(struct LogicalChannelConfig__ul_SpecificParameters__allowedSCS_List, _asn_ctx),
	1,	/* XER encoding is XMLValueList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_allowedSCS_List_40 = {
	"allowedSCS-List",
	"allowedSCS-List",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_allowedSCS_List_tags_40,
	sizeof(asn_DEF_allowedSCS_List_tags_40)
		/sizeof(asn_DEF_allowedSCS_List_tags_40[0]) - 1, /* 1 */
	asn_DEF_allowedSCS_List_tags_40,	/* Same as above */
	sizeof(asn_DEF_allowedSCS_List_tags_40)
		/sizeof(asn_DEF_allowedSCS_List_tags_40[0]), /* 2 */
	{ &asn_OER_type_allowedSCS_List_constr_40, &asn_PER_type_allowedSCS_List_constr_40, SEQUENCE_OF_constraint },
	asn_MBR_allowedSCS_List_40,
	1,	/* Single element */
	&asn_SPC_allowedSCS_List_specs_40	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_maxPUSCH_Duration_value2enum_42[] = {
	{ 0,	6,	"ms0p02" },
	{ 1,	6,	"ms0p04" },
	{ 2,	8,	"ms0p0625" },
	{ 3,	7,	"ms0p125" },
	{ 4,	6,	"ms0p25" },
	{ 5,	5,	"ms0p5" },
	{ 6,	6,	"spare2" },
	{ 7,	6,	"spare1" }
};
static const unsigned int asn_MAP_maxPUSCH_Duration_enum2value_42[] = {
	0,	/* ms0p02(0) */
	1,	/* ms0p04(1) */
	2,	/* ms0p0625(2) */
	3,	/* ms0p125(3) */
	4,	/* ms0p25(4) */
	5,	/* ms0p5(5) */
	7,	/* spare1(7) */
	6	/* spare2(6) */
};
static const asn_INTEGER_specifics_t asn_SPC_maxPUSCH_Duration_specs_42 = {
	asn_MAP_maxPUSCH_Duration_value2enum_42,	/* "tag" => N; sorted by tag */
	asn_MAP_maxPUSCH_Duration_enum2value_42,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_maxPUSCH_Duration_tags_42[] = {
	(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_maxPUSCH_Duration_42 = {
	"maxPUSCH-Duration",
	"maxPUSCH-Duration",
	&asn_OP_NativeEnumerated,
	asn_DEF_maxPUSCH_Duration_tags_42,
	sizeof(asn_DEF_maxPUSCH_Duration_tags_42)
		/sizeof(asn_DEF_maxPUSCH_Duration_tags_42[0]) - 1, /* 1 */
	asn_DEF_maxPUSCH_Duration_tags_42,	/* Same as above */
	sizeof(asn_DEF_maxPUSCH_Duration_tags_42)
		/sizeof(asn_DEF_maxPUSCH_Duration_tags_42[0]), /* 2 */
	{ &asn_OER_type_maxPUSCH_Duration_constr_42, &asn_PER_type_maxPUSCH_Duration_constr_42, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_maxPUSCH_Duration_specs_42	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_configuredGrantType1Allowed_value2enum_51[] = {
	{ 0,	4,	"true" }
};
static const unsigned int asn_MAP_configuredGrantType1Allowed_enum2value_51[] = {
	0	/* true(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_configuredGrantType1Allowed_specs_51 = {
	asn_MAP_configuredGrantType1Allowed_value2enum_51,	/* "tag" => N; sorted by tag */
	asn_MAP_configuredGrantType1Allowed_enum2value_51,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_configuredGrantType1Allowed_tags_51[] = {
	(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_configuredGrantType1Allowed_51 = {
	"configuredGrantType1Allowed",
	"configuredGrantType1Allowed",
	&asn_OP_NativeEnumerated,
	asn_DEF_configuredGrantType1Allowed_tags_51,
	sizeof(asn_DEF_configuredGrantType1Allowed_tags_51)
		/sizeof(asn_DEF_configuredGrantType1Allowed_tags_51[0]) - 1, /* 1 */
	asn_DEF_configuredGrantType1Allowed_tags_51,	/* Same as above */
	sizeof(asn_DEF_configuredGrantType1Allowed_tags_51)
		/sizeof(asn_DEF_configuredGrantType1Allowed_tags_51[0]), /* 2 */
	{ &asn_OER_type_configuredGrantType1Allowed_constr_51, &asn_PER_type_configuredGrantType1Allowed_constr_51, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_configuredGrantType1Allowed_specs_51	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_bitRateQueryProhibitTimer_value2enum_58[] = {
	{ 0,	2,	"s0" },
	{ 1,	6,	"s0dot4" },
	{ 2,	6,	"s0dot8" },
	{ 3,	6,	"s1dot6" },
	{ 4,	2,	"s3" },
	{ 5,	2,	"s6" },
	{ 6,	3,	"s12" },
	{ 7,	3,	"s30" }
};
static const unsigned int asn_MAP_bitRateQueryProhibitTimer_enum2value_58[] = {
	0,	/* s0(0) */
	1,	/* s0dot4(1) */
	2,	/* s0dot8(2) */
	6,	/* s12(6) */
	3,	/* s1dot6(3) */
	4,	/* s3(4) */
	7,	/* s30(7) */
	5	/* s6(5) */
};
static const asn_INTEGER_specifics_t asn_SPC_bitRateQueryProhibitTimer_specs_58 = {
	asn_MAP_bitRateQueryProhibitTimer_value2enum_58,	/* "tag" => N; sorted by tag */
	asn_MAP_bitRateQueryProhibitTimer_enum2value_58,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_bitRateQueryProhibitTimer_tags_58[] = {
	(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_bitRateQueryProhibitTimer_58 = {
	"bitRateQueryProhibitTimer",
	"bitRateQueryProhibitTimer",
	&asn_OP_NativeEnumerated,
	asn_DEF_bitRateQueryProhibitTimer_tags_58,
	sizeof(asn_DEF_bitRateQueryProhibitTimer_tags_58)
		/sizeof(asn_DEF_bitRateQueryProhibitTimer_tags_58[0]) - 1, /* 1 */
	asn_DEF_bitRateQueryProhibitTimer_tags_58,	/* Same as above */
	sizeof(asn_DEF_bitRateQueryProhibitTimer_tags_58)
		/sizeof(asn_DEF_bitRateQueryProhibitTimer_tags_58[0]), /* 2 */
	{ &asn_OER_type_bitRateQueryProhibitTimer_constr_58, &asn_PER_type_bitRateQueryProhibitTimer_constr_58, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_bitRateQueryProhibitTimer_specs_58	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_ul_SpecificParameters_2[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, priority),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_priority_constr_3, &asn_PER_memb_priority_constr_3,  memb_priority_constraint_2 },
		0, 0, /* No default value */
		"priority"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, prioritisedBitRate),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_prioritisedBitRate_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"prioritisedBitRate"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, bucketSizeDuration),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_bucketSizeDuration_21,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"bucketSizeDuration"
		},
	{ ATF_POINTER, 6, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, allowedServingCells),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		0,
		&asn_DEF_allowedServingCells_38,
		0,
		{ &asn_OER_memb_allowedServingCells_constr_38, &asn_PER_memb_allowedServingCells_constr_38,  memb_allowedServingCells_constraint_2 },
		0, 0, /* No default value */
		"allowedServingCells"
		},
	{ ATF_POINTER, 5, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, allowedSCS_List),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		0,
		&asn_DEF_allowedSCS_List_40,
		0,
		{ &asn_OER_memb_allowedSCS_List_constr_40, &asn_PER_memb_allowedSCS_List_constr_40,  memb_allowedSCS_List_constraint_2 },
		0, 0, /* No default value */
		"allowedSCS-List"
		},
	{ ATF_POINTER, 4, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, maxPUSCH_Duration),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_maxPUSCH_Duration_42,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxPUSCH-Duration"
		},
	{ ATF_POINTER, 3, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, configuredGrantType1Allowed),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_configuredGrantType1Allowed_51,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"configuredGrantType1Allowed"
		},
	{ ATF_POINTER, 2, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, logicalChannelGroup),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_logicalChannelGroup_constr_53, &asn_PER_memb_logicalChannelGroup_constr_53,  memb_logicalChannelGroup_constraint_2 },
		0, 0, /* No default value */
		"logicalChannelGroup"
		},
	{ ATF_POINTER, 1, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, schedulingRequestID),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SchedulingRequestId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"schedulingRequestID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, logicalChannelSR_Mask),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"logicalChannelSR-Mask"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, logicalChannelSR_DelayTimerApplied),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"logicalChannelSR-DelayTimerApplied"
		},
	{ ATF_POINTER, 1, offsetof(struct LogicalChannelConfig__ul_SpecificParameters, bitRateQueryProhibitTimer),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_bitRateQueryProhibitTimer_58,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"bitRateQueryProhibitTimer"
		},
};
static const int asn_MAP_ul_SpecificParameters_oms_2[] = { 3, 4, 5, 6, 7, 8, 11 };
static const ber_tlv_tag_t asn_DEF_ul_SpecificParameters_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ul_SpecificParameters_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* priority */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* prioritisedBitRate */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* bucketSizeDuration */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* allowedServingCells */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* allowedSCS-List */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* maxPUSCH-Duration */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* configuredGrantType1Allowed */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* logicalChannelGroup */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* schedulingRequestID */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* logicalChannelSR-Mask */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* logicalChannelSR-DelayTimerApplied */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 } /* bitRateQueryProhibitTimer */
};
static asn_SEQUENCE_specifics_t asn_SPC_ul_SpecificParameters_specs_2 = {
	sizeof(struct LogicalChannelConfig__ul_SpecificParameters),
	offsetof(struct LogicalChannelConfig__ul_SpecificParameters, _asn_ctx),
	asn_MAP_ul_SpecificParameters_tag2el_2,
	12,	/* Count of tags in the map */
	asn_MAP_ul_SpecificParameters_oms_2,	/* Optional members */
	6, 1,	/* Root/Additions */
	11,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_ul_SpecificParameters_2 = {
	"ul-SpecificParameters",
	"ul-SpecificParameters",
	&asn_OP_SEQUENCE,
	asn_DEF_ul_SpecificParameters_tags_2,
	sizeof(asn_DEF_ul_SpecificParameters_tags_2)
		/sizeof(asn_DEF_ul_SpecificParameters_tags_2[0]) - 1, /* 1 */
	asn_DEF_ul_SpecificParameters_tags_2,	/* Same as above */
	sizeof(asn_DEF_ul_SpecificParameters_tags_2)
		/sizeof(asn_DEF_ul_SpecificParameters_tags_2[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_ul_SpecificParameters_2,
	12,	/* Elements count */
	&asn_SPC_ul_SpecificParameters_specs_2	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_LogicalChannelConfig_1[] = {
	{ ATF_POINTER, 1, offsetof(struct LogicalChannelConfig, ul_SpecificParameters),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_ul_SpecificParameters_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ul-SpecificParameters"
		},
};
static const int asn_MAP_LogicalChannelConfig_oms_1[] = { 0 };
static const ber_tlv_tag_t asn_DEF_LogicalChannelConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_LogicalChannelConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* ul-SpecificParameters */
};
asn_SEQUENCE_specifics_t asn_SPC_LogicalChannelConfig_specs_1 = {
	sizeof(struct LogicalChannelConfig),
	offsetof(struct LogicalChannelConfig, _asn_ctx),
	asn_MAP_LogicalChannelConfig_tag2el_1,
	1,	/* Count of tags in the map */
	asn_MAP_LogicalChannelConfig_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_LogicalChannelConfig = {
	"LogicalChannelConfig",
	"LogicalChannelConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_LogicalChannelConfig_tags_1,
	sizeof(asn_DEF_LogicalChannelConfig_tags_1)
		/sizeof(asn_DEF_LogicalChannelConfig_tags_1[0]), /* 1 */
	asn_DEF_LogicalChannelConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_LogicalChannelConfig_tags_1)
		/sizeof(asn_DEF_LogicalChannelConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_LogicalChannelConfig_1,
	1,	/* Elements count */
	&asn_SPC_LogicalChannelConfig_specs_1	/* Additional specs */
};

