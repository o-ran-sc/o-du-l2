/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#include "MeasAndMobParametersXDD-Diff.h"

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
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_type_intraAndInterF_MeasAndReport_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_type_eventA_MeasAndReport_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_type_handoverInterF_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_type_handoverLTE_constr_10 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_type_handover_eLTE_constr_12 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
static const asn_INTEGER_enum_map_t asn_MAP_intraAndInterF_MeasAndReport_value2enum_2[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_intraAndInterF_MeasAndReport_enum2value_2[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_intraAndInterF_MeasAndReport_specs_2 = {
	asn_MAP_intraAndInterF_MeasAndReport_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_intraAndInterF_MeasAndReport_enum2value_2,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_intraAndInterF_MeasAndReport_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_intraAndInterF_MeasAndReport_2 = {
	"intraAndInterF-MeasAndReport",
	"intraAndInterF-MeasAndReport",
	&asn_OP_NativeEnumerated,
	asn_DEF_intraAndInterF_MeasAndReport_tags_2,
	sizeof(asn_DEF_intraAndInterF_MeasAndReport_tags_2)
		/sizeof(asn_DEF_intraAndInterF_MeasAndReport_tags_2[0]) - 1, /* 1 */
	asn_DEF_intraAndInterF_MeasAndReport_tags_2,	/* Same as above */
	sizeof(asn_DEF_intraAndInterF_MeasAndReport_tags_2)
		/sizeof(asn_DEF_intraAndInterF_MeasAndReport_tags_2[0]), /* 2 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_intraAndInterF_MeasAndReport_constr_2,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_intraAndInterF_MeasAndReport_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_eventA_MeasAndReport_value2enum_4[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_eventA_MeasAndReport_enum2value_4[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_eventA_MeasAndReport_specs_4 = {
	asn_MAP_eventA_MeasAndReport_value2enum_4,	/* "tag" => N; sorted by tag */
	asn_MAP_eventA_MeasAndReport_enum2value_4,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_eventA_MeasAndReport_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_eventA_MeasAndReport_4 = {
	"eventA-MeasAndReport",
	"eventA-MeasAndReport",
	&asn_OP_NativeEnumerated,
	asn_DEF_eventA_MeasAndReport_tags_4,
	sizeof(asn_DEF_eventA_MeasAndReport_tags_4)
		/sizeof(asn_DEF_eventA_MeasAndReport_tags_4[0]) - 1, /* 1 */
	asn_DEF_eventA_MeasAndReport_tags_4,	/* Same as above */
	sizeof(asn_DEF_eventA_MeasAndReport_tags_4)
		/sizeof(asn_DEF_eventA_MeasAndReport_tags_4[0]), /* 2 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_eventA_MeasAndReport_constr_4,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_eventA_MeasAndReport_specs_4	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_handoverInterF_value2enum_8[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_handoverInterF_enum2value_8[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_handoverInterF_specs_8 = {
	asn_MAP_handoverInterF_value2enum_8,	/* "tag" => N; sorted by tag */
	asn_MAP_handoverInterF_enum2value_8,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_handoverInterF_tags_8[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_handoverInterF_8 = {
	"handoverInterF",
	"handoverInterF",
	&asn_OP_NativeEnumerated,
	asn_DEF_handoverInterF_tags_8,
	sizeof(asn_DEF_handoverInterF_tags_8)
		/sizeof(asn_DEF_handoverInterF_tags_8[0]) - 1, /* 1 */
	asn_DEF_handoverInterF_tags_8,	/* Same as above */
	sizeof(asn_DEF_handoverInterF_tags_8)
		/sizeof(asn_DEF_handoverInterF_tags_8[0]), /* 2 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_handoverInterF_constr_8,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_handoverInterF_specs_8	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_handoverLTE_value2enum_10[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_handoverLTE_enum2value_10[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_handoverLTE_specs_10 = {
	asn_MAP_handoverLTE_value2enum_10,	/* "tag" => N; sorted by tag */
	asn_MAP_handoverLTE_enum2value_10,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_handoverLTE_tags_10[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_handoverLTE_10 = {
	"handoverLTE",
	"handoverLTE",
	&asn_OP_NativeEnumerated,
	asn_DEF_handoverLTE_tags_10,
	sizeof(asn_DEF_handoverLTE_tags_10)
		/sizeof(asn_DEF_handoverLTE_tags_10[0]) - 1, /* 1 */
	asn_DEF_handoverLTE_tags_10,	/* Same as above */
	sizeof(asn_DEF_handoverLTE_tags_10)
		/sizeof(asn_DEF_handoverLTE_tags_10[0]), /* 2 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_handoverLTE_constr_10,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_handoverLTE_specs_10	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_handover_eLTE_value2enum_12[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_handover_eLTE_enum2value_12[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_handover_eLTE_specs_12 = {
	asn_MAP_handover_eLTE_value2enum_12,	/* "tag" => N; sorted by tag */
	asn_MAP_handover_eLTE_enum2value_12,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_handover_eLTE_tags_12[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_handover_eLTE_12 = {
	"handover-eLTE",
	"handover-eLTE",
	&asn_OP_NativeEnumerated,
	asn_DEF_handover_eLTE_tags_12,
	sizeof(asn_DEF_handover_eLTE_tags_12)
		/sizeof(asn_DEF_handover_eLTE_tags_12[0]) - 1, /* 1 */
	asn_DEF_handover_eLTE_tags_12,	/* Same as above */
	sizeof(asn_DEF_handover_eLTE_tags_12)
		/sizeof(asn_DEF_handover_eLTE_tags_12[0]), /* 2 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_handover_eLTE_constr_12,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_handover_eLTE_specs_12	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_ext1_7[] = {
	{ ATF_POINTER, 3, offsetof(struct MeasAndMobParametersXDD_Diff__ext1, handoverInterF),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_handoverInterF_8,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"handoverInterF"
		},
	{ ATF_POINTER, 2, offsetof(struct MeasAndMobParametersXDD_Diff__ext1, handoverLTE),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_handoverLTE_10,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"handoverLTE"
		},
	{ ATF_POINTER, 1, offsetof(struct MeasAndMobParametersXDD_Diff__ext1, handover_eLTE),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_handover_eLTE_12,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"handover-eLTE"
		},
};
static const int asn_MAP_ext1_oms_7[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_ext1_tags_7[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ext1_tag2el_7[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* handoverInterF */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* handoverLTE */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* handover-eLTE */
};
static asn_SEQUENCE_specifics_t asn_SPC_ext1_specs_7 = {
	sizeof(struct MeasAndMobParametersXDD_Diff__ext1),
	offsetof(struct MeasAndMobParametersXDD_Diff__ext1, _asn_ctx),
	asn_MAP_ext1_tag2el_7,
	3,	/* Count of tags in the map */
	asn_MAP_ext1_oms_7,	/* Optional members */
	3, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_ext1_7 = {
	"ext1",
	"ext1",
	&asn_OP_SEQUENCE,
	asn_DEF_ext1_tags_7,
	sizeof(asn_DEF_ext1_tags_7)
		/sizeof(asn_DEF_ext1_tags_7[0]) - 1, /* 1 */
	asn_DEF_ext1_tags_7,	/* Same as above */
	sizeof(asn_DEF_ext1_tags_7)
		/sizeof(asn_DEF_ext1_tags_7[0]), /* 2 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		SEQUENCE_constraint
	},
	asn_MBR_ext1_7,
	3,	/* Elements count */
	&asn_SPC_ext1_specs_7	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_MeasAndMobParametersXDD_Diff_1[] = {
	{ ATF_POINTER, 3, offsetof(struct MeasAndMobParametersXDD_Diff, intraAndInterF_MeasAndReport),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_intraAndInterF_MeasAndReport_2,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"intraAndInterF-MeasAndReport"
		},
	{ ATF_POINTER, 2, offsetof(struct MeasAndMobParametersXDD_Diff, eventA_MeasAndReport),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_eventA_MeasAndReport_4,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"eventA-MeasAndReport"
		},
	{ ATF_POINTER, 1, offsetof(struct MeasAndMobParametersXDD_Diff, ext1),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_ext1_7,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"ext1"
		},
};
static const int asn_MAP_MeasAndMobParametersXDD_Diff_oms_1[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_MeasAndMobParametersXDD_Diff_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_MeasAndMobParametersXDD_Diff_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* intraAndInterF-MeasAndReport */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* eventA-MeasAndReport */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* ext1 */
};
asn_SEQUENCE_specifics_t asn_SPC_MeasAndMobParametersXDD_Diff_specs_1 = {
	sizeof(struct MeasAndMobParametersXDD_Diff),
	offsetof(struct MeasAndMobParametersXDD_Diff, _asn_ctx),
	asn_MAP_MeasAndMobParametersXDD_Diff_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_MeasAndMobParametersXDD_Diff_oms_1,	/* Optional members */
	2, 1,	/* Root/Additions */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_MeasAndMobParametersXDD_Diff = {
	"MeasAndMobParametersXDD-Diff",
	"MeasAndMobParametersXDD-Diff",
	&asn_OP_SEQUENCE,
	asn_DEF_MeasAndMobParametersXDD_Diff_tags_1,
	sizeof(asn_DEF_MeasAndMobParametersXDD_Diff_tags_1)
		/sizeof(asn_DEF_MeasAndMobParametersXDD_Diff_tags_1[0]), /* 1 */
	asn_DEF_MeasAndMobParametersXDD_Diff_tags_1,	/* Same as above */
	sizeof(asn_DEF_MeasAndMobParametersXDD_Diff_tags_1)
		/sizeof(asn_DEF_MeasAndMobParametersXDD_Diff_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		SEQUENCE_constraint
	},
	asn_MBR_MeasAndMobParametersXDD_Diff_1,
	3,	/* Elements count */
	&asn_SPC_MeasAndMobParametersXDD_Diff_specs_1	/* Additional specs */
};

