/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "CA-ParametersNR.h"

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
static asn_oer_constraints_t asn_OER_type_multipleTimingAdvances_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_multipleTimingAdvances_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_parallelTxSRS_PUCCH_PUSCH_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_parallelTxSRS_PUCCH_PUSCH_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_parallelTxPRACH_SRS_PUCCH_PUSCH_constr_6 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_parallelTxPRACH_SRS_PUCCH_PUSCH_constr_6 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_simultaneousRxTxInterBandCA_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_simultaneousRxTxInterBandCA_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_simultaneousRxTxSUL_constr_10 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_simultaneousRxTxSUL_constr_10 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_diffNumerologyAcrossPUCCH_Group_constr_12 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_diffNumerologyAcrossPUCCH_Group_constr_12 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_diffNumerologyWithinPUCCH_Group_constr_14 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_diffNumerologyWithinPUCCH_Group_constr_14 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_supportedNumberTAG_constr_16 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_supportedNumberTAG_constr_16 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_multipleTimingAdvances_value2enum_2[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_multipleTimingAdvances_enum2value_2[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_multipleTimingAdvances_specs_2 = {
	asn_MAP_multipleTimingAdvances_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_multipleTimingAdvances_enum2value_2,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_multipleTimingAdvances_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_multipleTimingAdvances_2 = {
	"multipleTimingAdvances",
	"multipleTimingAdvances",
	&asn_OP_NativeEnumerated,
	asn_DEF_multipleTimingAdvances_tags_2,
	sizeof(asn_DEF_multipleTimingAdvances_tags_2)
		/sizeof(asn_DEF_multipleTimingAdvances_tags_2[0]) - 1, /* 1 */
	asn_DEF_multipleTimingAdvances_tags_2,	/* Same as above */
	sizeof(asn_DEF_multipleTimingAdvances_tags_2)
		/sizeof(asn_DEF_multipleTimingAdvances_tags_2[0]), /* 2 */
	{ &asn_OER_type_multipleTimingAdvances_constr_2, &asn_PER_type_multipleTimingAdvances_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_multipleTimingAdvances_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_parallelTxSRS_PUCCH_PUSCH_value2enum_4[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_parallelTxSRS_PUCCH_PUSCH_enum2value_4[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_parallelTxSRS_PUCCH_PUSCH_specs_4 = {
	asn_MAP_parallelTxSRS_PUCCH_PUSCH_value2enum_4,	/* "tag" => N; sorted by tag */
	asn_MAP_parallelTxSRS_PUCCH_PUSCH_enum2value_4,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_parallelTxSRS_PUCCH_PUSCH_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_parallelTxSRS_PUCCH_PUSCH_4 = {
	"parallelTxSRS-PUCCH-PUSCH",
	"parallelTxSRS-PUCCH-PUSCH",
	&asn_OP_NativeEnumerated,
	asn_DEF_parallelTxSRS_PUCCH_PUSCH_tags_4,
	sizeof(asn_DEF_parallelTxSRS_PUCCH_PUSCH_tags_4)
		/sizeof(asn_DEF_parallelTxSRS_PUCCH_PUSCH_tags_4[0]) - 1, /* 1 */
	asn_DEF_parallelTxSRS_PUCCH_PUSCH_tags_4,	/* Same as above */
	sizeof(asn_DEF_parallelTxSRS_PUCCH_PUSCH_tags_4)
		/sizeof(asn_DEF_parallelTxSRS_PUCCH_PUSCH_tags_4[0]), /* 2 */
	{ &asn_OER_type_parallelTxSRS_PUCCH_PUSCH_constr_4, &asn_PER_type_parallelTxSRS_PUCCH_PUSCH_constr_4, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_parallelTxSRS_PUCCH_PUSCH_specs_4	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_parallelTxPRACH_SRS_PUCCH_PUSCH_value2enum_6[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_parallelTxPRACH_SRS_PUCCH_PUSCH_enum2value_6[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_parallelTxPRACH_SRS_PUCCH_PUSCH_specs_6 = {
	asn_MAP_parallelTxPRACH_SRS_PUCCH_PUSCH_value2enum_6,	/* "tag" => N; sorted by tag */
	asn_MAP_parallelTxPRACH_SRS_PUCCH_PUSCH_enum2value_6,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_parallelTxPRACH_SRS_PUCCH_PUSCH_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_parallelTxPRACH_SRS_PUCCH_PUSCH_6 = {
	"parallelTxPRACH-SRS-PUCCH-PUSCH",
	"parallelTxPRACH-SRS-PUCCH-PUSCH",
	&asn_OP_NativeEnumerated,
	asn_DEF_parallelTxPRACH_SRS_PUCCH_PUSCH_tags_6,
	sizeof(asn_DEF_parallelTxPRACH_SRS_PUCCH_PUSCH_tags_6)
		/sizeof(asn_DEF_parallelTxPRACH_SRS_PUCCH_PUSCH_tags_6[0]) - 1, /* 1 */
	asn_DEF_parallelTxPRACH_SRS_PUCCH_PUSCH_tags_6,	/* Same as above */
	sizeof(asn_DEF_parallelTxPRACH_SRS_PUCCH_PUSCH_tags_6)
		/sizeof(asn_DEF_parallelTxPRACH_SRS_PUCCH_PUSCH_tags_6[0]), /* 2 */
	{ &asn_OER_type_parallelTxPRACH_SRS_PUCCH_PUSCH_constr_6, &asn_PER_type_parallelTxPRACH_SRS_PUCCH_PUSCH_constr_6, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_parallelTxPRACH_SRS_PUCCH_PUSCH_specs_6	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_simultaneousRxTxInterBandCA_value2enum_8[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_simultaneousRxTxInterBandCA_enum2value_8[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_simultaneousRxTxInterBandCA_specs_8 = {
	asn_MAP_simultaneousRxTxInterBandCA_value2enum_8,	/* "tag" => N; sorted by tag */
	asn_MAP_simultaneousRxTxInterBandCA_enum2value_8,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_simultaneousRxTxInterBandCA_tags_8[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_simultaneousRxTxInterBandCA_8 = {
	"simultaneousRxTxInterBandCA",
	"simultaneousRxTxInterBandCA",
	&asn_OP_NativeEnumerated,
	asn_DEF_simultaneousRxTxInterBandCA_tags_8,
	sizeof(asn_DEF_simultaneousRxTxInterBandCA_tags_8)
		/sizeof(asn_DEF_simultaneousRxTxInterBandCA_tags_8[0]) - 1, /* 1 */
	asn_DEF_simultaneousRxTxInterBandCA_tags_8,	/* Same as above */
	sizeof(asn_DEF_simultaneousRxTxInterBandCA_tags_8)
		/sizeof(asn_DEF_simultaneousRxTxInterBandCA_tags_8[0]), /* 2 */
	{ &asn_OER_type_simultaneousRxTxInterBandCA_constr_8, &asn_PER_type_simultaneousRxTxInterBandCA_constr_8, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_simultaneousRxTxInterBandCA_specs_8	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_simultaneousRxTxSUL_value2enum_10[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_simultaneousRxTxSUL_enum2value_10[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_simultaneousRxTxSUL_specs_10 = {
	asn_MAP_simultaneousRxTxSUL_value2enum_10,	/* "tag" => N; sorted by tag */
	asn_MAP_simultaneousRxTxSUL_enum2value_10,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_simultaneousRxTxSUL_tags_10[] = {
	(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_simultaneousRxTxSUL_10 = {
	"simultaneousRxTxSUL",
	"simultaneousRxTxSUL",
	&asn_OP_NativeEnumerated,
	asn_DEF_simultaneousRxTxSUL_tags_10,
	sizeof(asn_DEF_simultaneousRxTxSUL_tags_10)
		/sizeof(asn_DEF_simultaneousRxTxSUL_tags_10[0]) - 1, /* 1 */
	asn_DEF_simultaneousRxTxSUL_tags_10,	/* Same as above */
	sizeof(asn_DEF_simultaneousRxTxSUL_tags_10)
		/sizeof(asn_DEF_simultaneousRxTxSUL_tags_10[0]), /* 2 */
	{ &asn_OER_type_simultaneousRxTxSUL_constr_10, &asn_PER_type_simultaneousRxTxSUL_constr_10, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_simultaneousRxTxSUL_specs_10	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_diffNumerologyAcrossPUCCH_Group_value2enum_12[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_diffNumerologyAcrossPUCCH_Group_enum2value_12[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_diffNumerologyAcrossPUCCH_Group_specs_12 = {
	asn_MAP_diffNumerologyAcrossPUCCH_Group_value2enum_12,	/* "tag" => N; sorted by tag */
	asn_MAP_diffNumerologyAcrossPUCCH_Group_enum2value_12,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_diffNumerologyAcrossPUCCH_Group_tags_12[] = {
	(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_diffNumerologyAcrossPUCCH_Group_12 = {
	"diffNumerologyAcrossPUCCH-Group",
	"diffNumerologyAcrossPUCCH-Group",
	&asn_OP_NativeEnumerated,
	asn_DEF_diffNumerologyAcrossPUCCH_Group_tags_12,
	sizeof(asn_DEF_diffNumerologyAcrossPUCCH_Group_tags_12)
		/sizeof(asn_DEF_diffNumerologyAcrossPUCCH_Group_tags_12[0]) - 1, /* 1 */
	asn_DEF_diffNumerologyAcrossPUCCH_Group_tags_12,	/* Same as above */
	sizeof(asn_DEF_diffNumerologyAcrossPUCCH_Group_tags_12)
		/sizeof(asn_DEF_diffNumerologyAcrossPUCCH_Group_tags_12[0]), /* 2 */
	{ &asn_OER_type_diffNumerologyAcrossPUCCH_Group_constr_12, &asn_PER_type_diffNumerologyAcrossPUCCH_Group_constr_12, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_diffNumerologyAcrossPUCCH_Group_specs_12	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_diffNumerologyWithinPUCCH_Group_value2enum_14[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_diffNumerologyWithinPUCCH_Group_enum2value_14[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_diffNumerologyWithinPUCCH_Group_specs_14 = {
	asn_MAP_diffNumerologyWithinPUCCH_Group_value2enum_14,	/* "tag" => N; sorted by tag */
	asn_MAP_diffNumerologyWithinPUCCH_Group_enum2value_14,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_diffNumerologyWithinPUCCH_Group_tags_14[] = {
	(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_diffNumerologyWithinPUCCH_Group_14 = {
	"diffNumerologyWithinPUCCH-Group",
	"diffNumerologyWithinPUCCH-Group",
	&asn_OP_NativeEnumerated,
	asn_DEF_diffNumerologyWithinPUCCH_Group_tags_14,
	sizeof(asn_DEF_diffNumerologyWithinPUCCH_Group_tags_14)
		/sizeof(asn_DEF_diffNumerologyWithinPUCCH_Group_tags_14[0]) - 1, /* 1 */
	asn_DEF_diffNumerologyWithinPUCCH_Group_tags_14,	/* Same as above */
	sizeof(asn_DEF_diffNumerologyWithinPUCCH_Group_tags_14)
		/sizeof(asn_DEF_diffNumerologyWithinPUCCH_Group_tags_14[0]), /* 2 */
	{ &asn_OER_type_diffNumerologyWithinPUCCH_Group_constr_14, &asn_PER_type_diffNumerologyWithinPUCCH_Group_constr_14, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_diffNumerologyWithinPUCCH_Group_specs_14	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_supportedNumberTAG_value2enum_16[] = {
	{ 0,	2,	"n2" },
	{ 1,	2,	"n3" },
	{ 2,	2,	"n4" }
};
static const unsigned int asn_MAP_supportedNumberTAG_enum2value_16[] = {
	0,	/* n2(0) */
	1,	/* n3(1) */
	2	/* n4(2) */
};
static const asn_INTEGER_specifics_t asn_SPC_supportedNumberTAG_specs_16 = {
	asn_MAP_supportedNumberTAG_value2enum_16,	/* "tag" => N; sorted by tag */
	asn_MAP_supportedNumberTAG_enum2value_16,	/* N => "tag"; sorted by N */
	3,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_supportedNumberTAG_tags_16[] = {
	(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_supportedNumberTAG_16 = {
	"supportedNumberTAG",
	"supportedNumberTAG",
	&asn_OP_NativeEnumerated,
	asn_DEF_supportedNumberTAG_tags_16,
	sizeof(asn_DEF_supportedNumberTAG_tags_16)
		/sizeof(asn_DEF_supportedNumberTAG_tags_16[0]) - 1, /* 1 */
	asn_DEF_supportedNumberTAG_tags_16,	/* Same as above */
	sizeof(asn_DEF_supportedNumberTAG_tags_16)
		/sizeof(asn_DEF_supportedNumberTAG_tags_16[0]), /* 2 */
	{ &asn_OER_type_supportedNumberTAG_constr_16, &asn_PER_type_supportedNumberTAG_constr_16, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_supportedNumberTAG_specs_16	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_CA_ParametersNR_1[] = {
	{ ATF_POINTER, 8, offsetof(struct CA_ParametersNR, multipleTimingAdvances),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_multipleTimingAdvances_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"multipleTimingAdvances"
		},
	{ ATF_POINTER, 7, offsetof(struct CA_ParametersNR, parallelTxSRS_PUCCH_PUSCH),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_parallelTxSRS_PUCCH_PUSCH_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"parallelTxSRS-PUCCH-PUSCH"
		},
	{ ATF_POINTER, 6, offsetof(struct CA_ParametersNR, parallelTxPRACH_SRS_PUCCH_PUSCH),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_parallelTxPRACH_SRS_PUCCH_PUSCH_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"parallelTxPRACH-SRS-PUCCH-PUSCH"
		},
	{ ATF_POINTER, 5, offsetof(struct CA_ParametersNR, simultaneousRxTxInterBandCA),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_simultaneousRxTxInterBandCA_8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"simultaneousRxTxInterBandCA"
		},
	{ ATF_POINTER, 4, offsetof(struct CA_ParametersNR, simultaneousRxTxSUL),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_simultaneousRxTxSUL_10,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"simultaneousRxTxSUL"
		},
	{ ATF_POINTER, 3, offsetof(struct CA_ParametersNR, diffNumerologyAcrossPUCCH_Group),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_diffNumerologyAcrossPUCCH_Group_12,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"diffNumerologyAcrossPUCCH-Group"
		},
	{ ATF_POINTER, 2, offsetof(struct CA_ParametersNR, diffNumerologyWithinPUCCH_Group),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_diffNumerologyWithinPUCCH_Group_14,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"diffNumerologyWithinPUCCH-Group"
		},
	{ ATF_POINTER, 1, offsetof(struct CA_ParametersNR, supportedNumberTAG),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_supportedNumberTAG_16,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"supportedNumberTAG"
		},
};
static const int asn_MAP_CA_ParametersNR_oms_1[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
static const ber_tlv_tag_t asn_DEF_CA_ParametersNR_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CA_ParametersNR_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* multipleTimingAdvances */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* parallelTxSRS-PUCCH-PUSCH */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* parallelTxPRACH-SRS-PUCCH-PUSCH */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* simultaneousRxTxInterBandCA */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* simultaneousRxTxSUL */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* diffNumerologyAcrossPUCCH-Group */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* diffNumerologyWithinPUCCH-Group */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 } /* supportedNumberTAG */
};
asn_SEQUENCE_specifics_t asn_SPC_CA_ParametersNR_specs_1 = {
	sizeof(struct CA_ParametersNR),
	offsetof(struct CA_ParametersNR, _asn_ctx),
	asn_MAP_CA_ParametersNR_tag2el_1,
	8,	/* Count of tags in the map */
	asn_MAP_CA_ParametersNR_oms_1,	/* Optional members */
	8, 0,	/* Root/Additions */
	8,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_CA_ParametersNR = {
	"CA-ParametersNR",
	"CA-ParametersNR",
	&asn_OP_SEQUENCE,
	asn_DEF_CA_ParametersNR_tags_1,
	sizeof(asn_DEF_CA_ParametersNR_tags_1)
		/sizeof(asn_DEF_CA_ParametersNR_tags_1[0]), /* 1 */
	asn_DEF_CA_ParametersNR_tags_1,	/* Same as above */
	sizeof(asn_DEF_CA_ParametersNR_tags_1)
		/sizeof(asn_DEF_CA_ParametersNR_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_CA_ParametersNR_1,
	8,	/* Elements count */
	&asn_SPC_CA_ParametersNR_specs_1	/* Additional specs */
};

