/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "PUSCH-PowerControl.h"

#include "P0-PUSCH-AlphaSet.h"
#include "PUSCH-PathlossReferenceRS.h"
#include "SRI-PUSCH-PowerControl.h"
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
memb_p0_NominalWithoutGrant_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -202 && value <= 24)) {
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
memb_p0_AlphaSets_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 30)) {
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
memb_pathlossReferenceRSToAddModList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 4)) {
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
memb_pathlossReferenceRSToReleaseList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 4)) {
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
memb_sri_PUSCH_MappingToAddModList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 16)) {
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
memb_sri_PUSCH_MappingToReleaseList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 16)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_tpc_Accumulation_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_tpc_Accumulation_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_p0_AlphaSets_constr_6 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..30)) */};
static asn_per_constraints_t asn_PER_type_p0_AlphaSets_constr_6 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  30 }	/* (SIZE(1..30)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_pathlossReferenceRSToAddModList_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..4)) */};
static asn_per_constraints_t asn_PER_type_pathlossReferenceRSToAddModList_constr_8 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 2,  2,  1,  4 }	/* (SIZE(1..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_pathlossReferenceRSToReleaseList_constr_10 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..4)) */};
static asn_per_constraints_t asn_PER_type_pathlossReferenceRSToReleaseList_constr_10 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 2,  2,  1,  4 }	/* (SIZE(1..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_twoPUSCH_PC_AdjustmentStates_constr_12 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_twoPUSCH_PC_AdjustmentStates_constr_12 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_deltaMCS_constr_14 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_deltaMCS_constr_14 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_sri_PUSCH_MappingToAddModList_constr_16 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..16)) */};
static asn_per_constraints_t asn_PER_type_sri_PUSCH_MappingToAddModList_constr_16 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 4,  4,  1,  16 }	/* (SIZE(1..16)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_sri_PUSCH_MappingToReleaseList_constr_18 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..16)) */};
static asn_per_constraints_t asn_PER_type_sri_PUSCH_MappingToReleaseList_constr_18 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 4,  4,  1,  16 }	/* (SIZE(1..16)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_p0_NominalWithoutGrant_constr_5 CC_NOTUSED = {
	{ 2, 0 }	/* (-202..24) */,
	-1};
static asn_per_constraints_t asn_PER_memb_p0_NominalWithoutGrant_constr_5 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 8,  8, -202,  24 }	/* (-202..24) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_p0_AlphaSets_constr_6 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..30)) */};
static asn_per_constraints_t asn_PER_memb_p0_AlphaSets_constr_6 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  30 }	/* (SIZE(1..30)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_pathlossReferenceRSToAddModList_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..4)) */};
static asn_per_constraints_t asn_PER_memb_pathlossReferenceRSToAddModList_constr_8 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 2,  2,  1,  4 }	/* (SIZE(1..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_pathlossReferenceRSToReleaseList_constr_10 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..4)) */};
static asn_per_constraints_t asn_PER_memb_pathlossReferenceRSToReleaseList_constr_10 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 2,  2,  1,  4 }	/* (SIZE(1..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_sri_PUSCH_MappingToAddModList_constr_16 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..16)) */};
static asn_per_constraints_t asn_PER_memb_sri_PUSCH_MappingToAddModList_constr_16 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 4,  4,  1,  16 }	/* (SIZE(1..16)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_sri_PUSCH_MappingToReleaseList_constr_18 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..16)) */};
static asn_per_constraints_t asn_PER_memb_sri_PUSCH_MappingToReleaseList_constr_18 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 4,  4,  1,  16 }	/* (SIZE(1..16)) */,
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_tpc_Accumulation_value2enum_2[] = {
	{ 0,	8,	"disabled" }
};
static const unsigned int asn_MAP_tpc_Accumulation_enum2value_2[] = {
	0	/* disabled(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_tpc_Accumulation_specs_2 = {
	asn_MAP_tpc_Accumulation_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_tpc_Accumulation_enum2value_2,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_tpc_Accumulation_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_tpc_Accumulation_2 = {
	"tpc-Accumulation",
	"tpc-Accumulation",
	&asn_OP_NativeEnumerated,
	asn_DEF_tpc_Accumulation_tags_2,
	sizeof(asn_DEF_tpc_Accumulation_tags_2)
		/sizeof(asn_DEF_tpc_Accumulation_tags_2[0]) - 1, /* 1 */
	asn_DEF_tpc_Accumulation_tags_2,	/* Same as above */
	sizeof(asn_DEF_tpc_Accumulation_tags_2)
		/sizeof(asn_DEF_tpc_Accumulation_tags_2[0]), /* 2 */
	{ &asn_OER_type_tpc_Accumulation_constr_2, &asn_PER_type_tpc_Accumulation_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_tpc_Accumulation_specs_2	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_p0_AlphaSets_6[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_P0_PUSCH_AlphaSet,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_p0_AlphaSets_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_p0_AlphaSets_specs_6 = {
	sizeof(struct PUSCH_PowerControl__p0_AlphaSets),
	offsetof(struct PUSCH_PowerControl__p0_AlphaSets, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_p0_AlphaSets_6 = {
	"p0-AlphaSets",
	"p0-AlphaSets",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_p0_AlphaSets_tags_6,
	sizeof(asn_DEF_p0_AlphaSets_tags_6)
		/sizeof(asn_DEF_p0_AlphaSets_tags_6[0]) - 1, /* 1 */
	asn_DEF_p0_AlphaSets_tags_6,	/* Same as above */
	sizeof(asn_DEF_p0_AlphaSets_tags_6)
		/sizeof(asn_DEF_p0_AlphaSets_tags_6[0]), /* 2 */
	{ &asn_OER_type_p0_AlphaSets_constr_6, &asn_PER_type_p0_AlphaSets_constr_6, SEQUENCE_OF_constraint },
	asn_MBR_p0_AlphaSets_6,
	1,	/* Single element */
	&asn_SPC_p0_AlphaSets_specs_6	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_pathlossReferenceRSToAddModList_8[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_PUSCH_PathlossReferenceRS,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_pathlossReferenceRSToAddModList_tags_8[] = {
	(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_pathlossReferenceRSToAddModList_specs_8 = {
	sizeof(struct PUSCH_PowerControl__pathlossReferenceRSToAddModList),
	offsetof(struct PUSCH_PowerControl__pathlossReferenceRSToAddModList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pathlossReferenceRSToAddModList_8 = {
	"pathlossReferenceRSToAddModList",
	"pathlossReferenceRSToAddModList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_pathlossReferenceRSToAddModList_tags_8,
	sizeof(asn_DEF_pathlossReferenceRSToAddModList_tags_8)
		/sizeof(asn_DEF_pathlossReferenceRSToAddModList_tags_8[0]) - 1, /* 1 */
	asn_DEF_pathlossReferenceRSToAddModList_tags_8,	/* Same as above */
	sizeof(asn_DEF_pathlossReferenceRSToAddModList_tags_8)
		/sizeof(asn_DEF_pathlossReferenceRSToAddModList_tags_8[0]), /* 2 */
	{ &asn_OER_type_pathlossReferenceRSToAddModList_constr_8, &asn_PER_type_pathlossReferenceRSToAddModList_constr_8, SEQUENCE_OF_constraint },
	asn_MBR_pathlossReferenceRSToAddModList_8,
	1,	/* Single element */
	&asn_SPC_pathlossReferenceRSToAddModList_specs_8	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_pathlossReferenceRSToReleaseList_10[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_PUSCH_PathlossReferenceRS_Id,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_pathlossReferenceRSToReleaseList_tags_10[] = {
	(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_pathlossReferenceRSToReleaseList_specs_10 = {
	sizeof(struct PUSCH_PowerControl__pathlossReferenceRSToReleaseList),
	offsetof(struct PUSCH_PowerControl__pathlossReferenceRSToReleaseList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pathlossReferenceRSToReleaseList_10 = {
	"pathlossReferenceRSToReleaseList",
	"pathlossReferenceRSToReleaseList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_pathlossReferenceRSToReleaseList_tags_10,
	sizeof(asn_DEF_pathlossReferenceRSToReleaseList_tags_10)
		/sizeof(asn_DEF_pathlossReferenceRSToReleaseList_tags_10[0]) - 1, /* 1 */
	asn_DEF_pathlossReferenceRSToReleaseList_tags_10,	/* Same as above */
	sizeof(asn_DEF_pathlossReferenceRSToReleaseList_tags_10)
		/sizeof(asn_DEF_pathlossReferenceRSToReleaseList_tags_10[0]), /* 2 */
	{ &asn_OER_type_pathlossReferenceRSToReleaseList_constr_10, &asn_PER_type_pathlossReferenceRSToReleaseList_constr_10, SEQUENCE_OF_constraint },
	asn_MBR_pathlossReferenceRSToReleaseList_10,
	1,	/* Single element */
	&asn_SPC_pathlossReferenceRSToReleaseList_specs_10	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_twoPUSCH_PC_AdjustmentStates_value2enum_12[] = {
	{ 0,	9,	"twoStates" }
};
static const unsigned int asn_MAP_twoPUSCH_PC_AdjustmentStates_enum2value_12[] = {
	0	/* twoStates(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_twoPUSCH_PC_AdjustmentStates_specs_12 = {
	asn_MAP_twoPUSCH_PC_AdjustmentStates_value2enum_12,	/* "tag" => N; sorted by tag */
	asn_MAP_twoPUSCH_PC_AdjustmentStates_enum2value_12,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_twoPUSCH_PC_AdjustmentStates_tags_12[] = {
	(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_twoPUSCH_PC_AdjustmentStates_12 = {
	"twoPUSCH-PC-AdjustmentStates",
	"twoPUSCH-PC-AdjustmentStates",
	&asn_OP_NativeEnumerated,
	asn_DEF_twoPUSCH_PC_AdjustmentStates_tags_12,
	sizeof(asn_DEF_twoPUSCH_PC_AdjustmentStates_tags_12)
		/sizeof(asn_DEF_twoPUSCH_PC_AdjustmentStates_tags_12[0]) - 1, /* 1 */
	asn_DEF_twoPUSCH_PC_AdjustmentStates_tags_12,	/* Same as above */
	sizeof(asn_DEF_twoPUSCH_PC_AdjustmentStates_tags_12)
		/sizeof(asn_DEF_twoPUSCH_PC_AdjustmentStates_tags_12[0]), /* 2 */
	{ &asn_OER_type_twoPUSCH_PC_AdjustmentStates_constr_12, &asn_PER_type_twoPUSCH_PC_AdjustmentStates_constr_12, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_twoPUSCH_PC_AdjustmentStates_specs_12	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_deltaMCS_value2enum_14[] = {
	{ 0,	7,	"enabled" }
};
static const unsigned int asn_MAP_deltaMCS_enum2value_14[] = {
	0	/* enabled(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_deltaMCS_specs_14 = {
	asn_MAP_deltaMCS_value2enum_14,	/* "tag" => N; sorted by tag */
	asn_MAP_deltaMCS_enum2value_14,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_deltaMCS_tags_14[] = {
	(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_deltaMCS_14 = {
	"deltaMCS",
	"deltaMCS",
	&asn_OP_NativeEnumerated,
	asn_DEF_deltaMCS_tags_14,
	sizeof(asn_DEF_deltaMCS_tags_14)
		/sizeof(asn_DEF_deltaMCS_tags_14[0]) - 1, /* 1 */
	asn_DEF_deltaMCS_tags_14,	/* Same as above */
	sizeof(asn_DEF_deltaMCS_tags_14)
		/sizeof(asn_DEF_deltaMCS_tags_14[0]), /* 2 */
	{ &asn_OER_type_deltaMCS_constr_14, &asn_PER_type_deltaMCS_constr_14, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_deltaMCS_specs_14	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_sri_PUSCH_MappingToAddModList_16[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_SRI_PUSCH_PowerControl,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_sri_PUSCH_MappingToAddModList_tags_16[] = {
	(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_sri_PUSCH_MappingToAddModList_specs_16 = {
	sizeof(struct PUSCH_PowerControl__sri_PUSCH_MappingToAddModList),
	offsetof(struct PUSCH_PowerControl__sri_PUSCH_MappingToAddModList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_sri_PUSCH_MappingToAddModList_16 = {
	"sri-PUSCH-MappingToAddModList",
	"sri-PUSCH-MappingToAddModList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_sri_PUSCH_MappingToAddModList_tags_16,
	sizeof(asn_DEF_sri_PUSCH_MappingToAddModList_tags_16)
		/sizeof(asn_DEF_sri_PUSCH_MappingToAddModList_tags_16[0]) - 1, /* 1 */
	asn_DEF_sri_PUSCH_MappingToAddModList_tags_16,	/* Same as above */
	sizeof(asn_DEF_sri_PUSCH_MappingToAddModList_tags_16)
		/sizeof(asn_DEF_sri_PUSCH_MappingToAddModList_tags_16[0]), /* 2 */
	{ &asn_OER_type_sri_PUSCH_MappingToAddModList_constr_16, &asn_PER_type_sri_PUSCH_MappingToAddModList_constr_16, SEQUENCE_OF_constraint },
	asn_MBR_sri_PUSCH_MappingToAddModList_16,
	1,	/* Single element */
	&asn_SPC_sri_PUSCH_MappingToAddModList_specs_16	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_sri_PUSCH_MappingToReleaseList_18[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_SRI_PUSCH_PowerControlId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_sri_PUSCH_MappingToReleaseList_tags_18[] = {
	(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_sri_PUSCH_MappingToReleaseList_specs_18 = {
	sizeof(struct PUSCH_PowerControl__sri_PUSCH_MappingToReleaseList),
	offsetof(struct PUSCH_PowerControl__sri_PUSCH_MappingToReleaseList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_sri_PUSCH_MappingToReleaseList_18 = {
	"sri-PUSCH-MappingToReleaseList",
	"sri-PUSCH-MappingToReleaseList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_sri_PUSCH_MappingToReleaseList_tags_18,
	sizeof(asn_DEF_sri_PUSCH_MappingToReleaseList_tags_18)
		/sizeof(asn_DEF_sri_PUSCH_MappingToReleaseList_tags_18[0]) - 1, /* 1 */
	asn_DEF_sri_PUSCH_MappingToReleaseList_tags_18,	/* Same as above */
	sizeof(asn_DEF_sri_PUSCH_MappingToReleaseList_tags_18)
		/sizeof(asn_DEF_sri_PUSCH_MappingToReleaseList_tags_18[0]), /* 2 */
	{ &asn_OER_type_sri_PUSCH_MappingToReleaseList_constr_18, &asn_PER_type_sri_PUSCH_MappingToReleaseList_constr_18, SEQUENCE_OF_constraint },
	asn_MBR_sri_PUSCH_MappingToReleaseList_18,
	1,	/* Single element */
	&asn_SPC_sri_PUSCH_MappingToReleaseList_specs_18	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_PUSCH_PowerControl_1[] = {
	{ ATF_POINTER, 10, offsetof(struct PUSCH_PowerControl, tpc_Accumulation),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_tpc_Accumulation_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"tpc-Accumulation"
		},
	{ ATF_POINTER, 9, offsetof(struct PUSCH_PowerControl, msg3_Alpha),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Alpha,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"msg3-Alpha"
		},
	{ ATF_POINTER, 8, offsetof(struct PUSCH_PowerControl, p0_NominalWithoutGrant),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_p0_NominalWithoutGrant_constr_5, &asn_PER_memb_p0_NominalWithoutGrant_constr_5,  memb_p0_NominalWithoutGrant_constraint_1 },
		0, 0, /* No default value */
		"p0-NominalWithoutGrant"
		},
	{ ATF_POINTER, 7, offsetof(struct PUSCH_PowerControl, p0_AlphaSets),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		0,
		&asn_DEF_p0_AlphaSets_6,
		0,
		{ &asn_OER_memb_p0_AlphaSets_constr_6, &asn_PER_memb_p0_AlphaSets_constr_6,  memb_p0_AlphaSets_constraint_1 },
		0, 0, /* No default value */
		"p0-AlphaSets"
		},
	{ ATF_POINTER, 6, offsetof(struct PUSCH_PowerControl, pathlossReferenceRSToAddModList),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		0,
		&asn_DEF_pathlossReferenceRSToAddModList_8,
		0,
		{ &asn_OER_memb_pathlossReferenceRSToAddModList_constr_8, &asn_PER_memb_pathlossReferenceRSToAddModList_constr_8,  memb_pathlossReferenceRSToAddModList_constraint_1 },
		0, 0, /* No default value */
		"pathlossReferenceRSToAddModList"
		},
	{ ATF_POINTER, 5, offsetof(struct PUSCH_PowerControl, pathlossReferenceRSToReleaseList),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		0,
		&asn_DEF_pathlossReferenceRSToReleaseList_10,
		0,
		{ &asn_OER_memb_pathlossReferenceRSToReleaseList_constr_10, &asn_PER_memb_pathlossReferenceRSToReleaseList_constr_10,  memb_pathlossReferenceRSToReleaseList_constraint_1 },
		0, 0, /* No default value */
		"pathlossReferenceRSToReleaseList"
		},
	{ ATF_POINTER, 4, offsetof(struct PUSCH_PowerControl, twoPUSCH_PC_AdjustmentStates),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_twoPUSCH_PC_AdjustmentStates_12,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"twoPUSCH-PC-AdjustmentStates"
		},
	{ ATF_POINTER, 3, offsetof(struct PUSCH_PowerControl, deltaMCS),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_deltaMCS_14,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"deltaMCS"
		},
	{ ATF_POINTER, 2, offsetof(struct PUSCH_PowerControl, sri_PUSCH_MappingToAddModList),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		0,
		&asn_DEF_sri_PUSCH_MappingToAddModList_16,
		0,
		{ &asn_OER_memb_sri_PUSCH_MappingToAddModList_constr_16, &asn_PER_memb_sri_PUSCH_MappingToAddModList_constr_16,  memb_sri_PUSCH_MappingToAddModList_constraint_1 },
		0, 0, /* No default value */
		"sri-PUSCH-MappingToAddModList"
		},
	{ ATF_POINTER, 1, offsetof(struct PUSCH_PowerControl, sri_PUSCH_MappingToReleaseList),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		0,
		&asn_DEF_sri_PUSCH_MappingToReleaseList_18,
		0,
		{ &asn_OER_memb_sri_PUSCH_MappingToReleaseList_constr_18, &asn_PER_memb_sri_PUSCH_MappingToReleaseList_constr_18,  memb_sri_PUSCH_MappingToReleaseList_constraint_1 },
		0, 0, /* No default value */
		"sri-PUSCH-MappingToReleaseList"
		},
};
static const int asn_MAP_PUSCH_PowerControl_oms_1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
static const ber_tlv_tag_t asn_DEF_PUSCH_PowerControl_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PUSCH_PowerControl_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* tpc-Accumulation */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* msg3-Alpha */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* p0-NominalWithoutGrant */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* p0-AlphaSets */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* pathlossReferenceRSToAddModList */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* pathlossReferenceRSToReleaseList */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* twoPUSCH-PC-AdjustmentStates */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* deltaMCS */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* sri-PUSCH-MappingToAddModList */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 } /* sri-PUSCH-MappingToReleaseList */
};
asn_SEQUENCE_specifics_t asn_SPC_PUSCH_PowerControl_specs_1 = {
	sizeof(struct PUSCH_PowerControl),
	offsetof(struct PUSCH_PowerControl, _asn_ctx),
	asn_MAP_PUSCH_PowerControl_tag2el_1,
	10,	/* Count of tags in the map */
	asn_MAP_PUSCH_PowerControl_oms_1,	/* Optional members */
	10, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_PUSCH_PowerControl = {
	"PUSCH-PowerControl",
	"PUSCH-PowerControl",
	&asn_OP_SEQUENCE,
	asn_DEF_PUSCH_PowerControl_tags_1,
	sizeof(asn_DEF_PUSCH_PowerControl_tags_1)
		/sizeof(asn_DEF_PUSCH_PowerControl_tags_1[0]), /* 1 */
	asn_DEF_PUSCH_PowerControl_tags_1,	/* Same as above */
	sizeof(asn_DEF_PUSCH_PowerControl_tags_1)
		/sizeof(asn_DEF_PUSCH_PowerControl_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_PUSCH_PowerControl_1,
	10,	/* Elements count */
	&asn_SPC_PUSCH_PowerControl_specs_1	/* Additional specs */
};

