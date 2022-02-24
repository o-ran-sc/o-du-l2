/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "EUTRA-ParametersCommon.h"

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
memb_modifiedMPR_BehaviorEUTRA_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size == 32)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_mfbi_EUTRA_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_mfbi_EUTRA_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_multiNS_Pmax_EUTRA_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_multiNS_Pmax_EUTRA_constr_5 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_rs_SINR_MeasEUTRA_constr_7 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_rs_SINR_MeasEUTRA_constr_7 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_modifiedMPR_BehaviorEUTRA_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	32	/* (SIZE(32..32)) */};
static asn_per_constraints_t asn_PER_memb_modifiedMPR_BehaviorEUTRA_constr_4 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  32,  32 }	/* (SIZE(32..32)) */,
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_mfbi_EUTRA_value2enum_2[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_mfbi_EUTRA_enum2value_2[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_mfbi_EUTRA_specs_2 = {
	asn_MAP_mfbi_EUTRA_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_mfbi_EUTRA_enum2value_2,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_mfbi_EUTRA_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_mfbi_EUTRA_2 = {
	"mfbi-EUTRA",
	"mfbi-EUTRA",
	&asn_OP_NativeEnumerated,
	asn_DEF_mfbi_EUTRA_tags_2,
	sizeof(asn_DEF_mfbi_EUTRA_tags_2)
		/sizeof(asn_DEF_mfbi_EUTRA_tags_2[0]) - 1, /* 1 */
	asn_DEF_mfbi_EUTRA_tags_2,	/* Same as above */
	sizeof(asn_DEF_mfbi_EUTRA_tags_2)
		/sizeof(asn_DEF_mfbi_EUTRA_tags_2[0]), /* 2 */
	{ &asn_OER_type_mfbi_EUTRA_constr_2, &asn_PER_type_mfbi_EUTRA_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_mfbi_EUTRA_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_multiNS_Pmax_EUTRA_value2enum_5[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_multiNS_Pmax_EUTRA_enum2value_5[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_multiNS_Pmax_EUTRA_specs_5 = {
	asn_MAP_multiNS_Pmax_EUTRA_value2enum_5,	/* "tag" => N; sorted by tag */
	asn_MAP_multiNS_Pmax_EUTRA_enum2value_5,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_multiNS_Pmax_EUTRA_tags_5[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_multiNS_Pmax_EUTRA_5 = {
	"multiNS-Pmax-EUTRA",
	"multiNS-Pmax-EUTRA",
	&asn_OP_NativeEnumerated,
	asn_DEF_multiNS_Pmax_EUTRA_tags_5,
	sizeof(asn_DEF_multiNS_Pmax_EUTRA_tags_5)
		/sizeof(asn_DEF_multiNS_Pmax_EUTRA_tags_5[0]) - 1, /* 1 */
	asn_DEF_multiNS_Pmax_EUTRA_tags_5,	/* Same as above */
	sizeof(asn_DEF_multiNS_Pmax_EUTRA_tags_5)
		/sizeof(asn_DEF_multiNS_Pmax_EUTRA_tags_5[0]), /* 2 */
	{ &asn_OER_type_multiNS_Pmax_EUTRA_constr_5, &asn_PER_type_multiNS_Pmax_EUTRA_constr_5, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_multiNS_Pmax_EUTRA_specs_5	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_rs_SINR_MeasEUTRA_value2enum_7[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_rs_SINR_MeasEUTRA_enum2value_7[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_rs_SINR_MeasEUTRA_specs_7 = {
	asn_MAP_rs_SINR_MeasEUTRA_value2enum_7,	/* "tag" => N; sorted by tag */
	asn_MAP_rs_SINR_MeasEUTRA_enum2value_7,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_rs_SINR_MeasEUTRA_tags_7[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_rs_SINR_MeasEUTRA_7 = {
	"rs-SINR-MeasEUTRA",
	"rs-SINR-MeasEUTRA",
	&asn_OP_NativeEnumerated,
	asn_DEF_rs_SINR_MeasEUTRA_tags_7,
	sizeof(asn_DEF_rs_SINR_MeasEUTRA_tags_7)
		/sizeof(asn_DEF_rs_SINR_MeasEUTRA_tags_7[0]) - 1, /* 1 */
	asn_DEF_rs_SINR_MeasEUTRA_tags_7,	/* Same as above */
	sizeof(asn_DEF_rs_SINR_MeasEUTRA_tags_7)
		/sizeof(asn_DEF_rs_SINR_MeasEUTRA_tags_7[0]), /* 2 */
	{ &asn_OER_type_rs_SINR_MeasEUTRA_constr_7, &asn_PER_type_rs_SINR_MeasEUTRA_constr_7, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_rs_SINR_MeasEUTRA_specs_7	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_EUTRA_ParametersCommon_1[] = {
	{ ATF_POINTER, 4, offsetof(struct EUTRA_ParametersCommon, mfbi_EUTRA),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_mfbi_EUTRA_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"mfbi-EUTRA"
		},
	{ ATF_POINTER, 3, offsetof(struct EUTRA_ParametersCommon, modifiedMPR_BehaviorEUTRA),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_modifiedMPR_BehaviorEUTRA_constr_4, &asn_PER_memb_modifiedMPR_BehaviorEUTRA_constr_4,  memb_modifiedMPR_BehaviorEUTRA_constraint_1 },
		0, 0, /* No default value */
		"modifiedMPR-BehaviorEUTRA"
		},
	{ ATF_POINTER, 2, offsetof(struct EUTRA_ParametersCommon, multiNS_Pmax_EUTRA),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_multiNS_Pmax_EUTRA_5,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"multiNS-Pmax-EUTRA"
		},
	{ ATF_POINTER, 1, offsetof(struct EUTRA_ParametersCommon, rs_SINR_MeasEUTRA),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_rs_SINR_MeasEUTRA_7,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rs-SINR-MeasEUTRA"
		},
};
static const int asn_MAP_EUTRA_ParametersCommon_oms_1[] = { 0, 1, 2, 3 };
static const ber_tlv_tag_t asn_DEF_EUTRA_ParametersCommon_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_EUTRA_ParametersCommon_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* mfbi-EUTRA */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* modifiedMPR-BehaviorEUTRA */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* multiNS-Pmax-EUTRA */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* rs-SINR-MeasEUTRA */
};
asn_SEQUENCE_specifics_t asn_SPC_EUTRA_ParametersCommon_specs_1 = {
	sizeof(struct EUTRA_ParametersCommon),
	offsetof(struct EUTRA_ParametersCommon, _asn_ctx),
	asn_MAP_EUTRA_ParametersCommon_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_EUTRA_ParametersCommon_oms_1,	/* Optional members */
	4, 0,	/* Root/Additions */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_EUTRA_ParametersCommon = {
	"EUTRA-ParametersCommon",
	"EUTRA-ParametersCommon",
	&asn_OP_SEQUENCE,
	asn_DEF_EUTRA_ParametersCommon_tags_1,
	sizeof(asn_DEF_EUTRA_ParametersCommon_tags_1)
		/sizeof(asn_DEF_EUTRA_ParametersCommon_tags_1[0]), /* 1 */
	asn_DEF_EUTRA_ParametersCommon_tags_1,	/* Same as above */
	sizeof(asn_DEF_EUTRA_ParametersCommon_tags_1)
		/sizeof(asn_DEF_EUTRA_ParametersCommon_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_EUTRA_ParametersCommon_1,
	4,	/* Elements count */
	&asn_SPC_EUTRA_ParametersCommon_specs_1	/* Additional specs */
};

