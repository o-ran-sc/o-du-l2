/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#include "CSI-IM-Resource.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_symbolLocation_p0_constraint_4(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0L && value <= 12L)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_symbolLocation_p1_constraint_13(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0L && value <= 13L)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_type_subcarrierLocation_p0_constr_5 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  5 }	/* (0..5) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_memb_symbolLocation_p0_constr_12 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  12 }	/* (0..12) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_type_subcarrierLocation_p1_constr_14 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_memb_symbolLocation_p1_constr_18 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  13 }	/* (0..13) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_type_csi_IM_ResourceElementPattern_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
static const asn_INTEGER_enum_map_t asn_MAP_subcarrierLocation_p0_value2enum_5[] = {
	{ 0,	2,	"s0" },
	{ 1,	2,	"s2" },
	{ 2,	2,	"s4" },
	{ 3,	2,	"s6" },
	{ 4,	2,	"s8" },
	{ 5,	3,	"s10" }
};
static const unsigned int asn_MAP_subcarrierLocation_p0_enum2value_5[] = {
	0,	/* s0(0) */
	5,	/* s10(5) */
	1,	/* s2(1) */
	2,	/* s4(2) */
	3,	/* s6(3) */
	4	/* s8(4) */
};
static const asn_INTEGER_specifics_t asn_SPC_subcarrierLocation_p0_specs_5 = {
	asn_MAP_subcarrierLocation_p0_value2enum_5,	/* "tag" => N; sorted by tag */
	asn_MAP_subcarrierLocation_p0_enum2value_5,	/* N => "tag"; sorted by N */
	6,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_subcarrierLocation_p0_tags_5[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_subcarrierLocation_p0_5 = {
	"subcarrierLocation-p0",
	"subcarrierLocation-p0",
	&asn_OP_NativeEnumerated,
	asn_DEF_subcarrierLocation_p0_tags_5,
	sizeof(asn_DEF_subcarrierLocation_p0_tags_5)
		/sizeof(asn_DEF_subcarrierLocation_p0_tags_5[0]) - 1, /* 1 */
	asn_DEF_subcarrierLocation_p0_tags_5,	/* Same as above */
	sizeof(asn_DEF_subcarrierLocation_p0_tags_5)
		/sizeof(asn_DEF_subcarrierLocation_p0_tags_5[0]), /* 2 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_subcarrierLocation_p0_constr_5,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_subcarrierLocation_p0_specs_5	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_pattern0_4[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern__pattern0, subcarrierLocation_p0),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_subcarrierLocation_p0_5,
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
		"subcarrierLocation-p0"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern__pattern0, symbolLocation_p0),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			&asn_PER_memb_symbolLocation_p0_constr_12,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			memb_symbolLocation_p0_constraint_4
		},
		0, 0, /* No default value */
		"symbolLocation-p0"
		},
};
static const ber_tlv_tag_t asn_DEF_pattern0_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_pattern0_tag2el_4[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* subcarrierLocation-p0 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* symbolLocation-p0 */
};
static asn_SEQUENCE_specifics_t asn_SPC_pattern0_specs_4 = {
	sizeof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern__pattern0),
	offsetof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern__pattern0, _asn_ctx),
	asn_MAP_pattern0_tag2el_4,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pattern0_4 = {
	"pattern0",
	"pattern0",
	&asn_OP_SEQUENCE,
	asn_DEF_pattern0_tags_4,
	sizeof(asn_DEF_pattern0_tags_4)
		/sizeof(asn_DEF_pattern0_tags_4[0]) - 1, /* 1 */
	asn_DEF_pattern0_tags_4,	/* Same as above */
	sizeof(asn_DEF_pattern0_tags_4)
		/sizeof(asn_DEF_pattern0_tags_4[0]), /* 2 */
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
	asn_MBR_pattern0_4,
	2,	/* Elements count */
	&asn_SPC_pattern0_specs_4	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_subcarrierLocation_p1_value2enum_14[] = {
	{ 0,	2,	"s0" },
	{ 1,	2,	"s4" },
	{ 2,	2,	"s8" }
};
static const unsigned int asn_MAP_subcarrierLocation_p1_enum2value_14[] = {
	0,	/* s0(0) */
	1,	/* s4(1) */
	2	/* s8(2) */
};
static const asn_INTEGER_specifics_t asn_SPC_subcarrierLocation_p1_specs_14 = {
	asn_MAP_subcarrierLocation_p1_value2enum_14,	/* "tag" => N; sorted by tag */
	asn_MAP_subcarrierLocation_p1_enum2value_14,	/* N => "tag"; sorted by N */
	3,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_subcarrierLocation_p1_tags_14[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_subcarrierLocation_p1_14 = {
	"subcarrierLocation-p1",
	"subcarrierLocation-p1",
	&asn_OP_NativeEnumerated,
	asn_DEF_subcarrierLocation_p1_tags_14,
	sizeof(asn_DEF_subcarrierLocation_p1_tags_14)
		/sizeof(asn_DEF_subcarrierLocation_p1_tags_14[0]) - 1, /* 1 */
	asn_DEF_subcarrierLocation_p1_tags_14,	/* Same as above */
	sizeof(asn_DEF_subcarrierLocation_p1_tags_14)
		/sizeof(asn_DEF_subcarrierLocation_p1_tags_14[0]), /* 2 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_subcarrierLocation_p1_constr_14,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_subcarrierLocation_p1_specs_14	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_pattern1_13[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern__pattern1, subcarrierLocation_p1),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_subcarrierLocation_p1_14,
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
		"subcarrierLocation-p1"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern__pattern1, symbolLocation_p1),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			&asn_PER_memb_symbolLocation_p1_constr_18,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			memb_symbolLocation_p1_constraint_13
		},
		0, 0, /* No default value */
		"symbolLocation-p1"
		},
};
static const ber_tlv_tag_t asn_DEF_pattern1_tags_13[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_pattern1_tag2el_13[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* subcarrierLocation-p1 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* symbolLocation-p1 */
};
static asn_SEQUENCE_specifics_t asn_SPC_pattern1_specs_13 = {
	sizeof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern__pattern1),
	offsetof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern__pattern1, _asn_ctx),
	asn_MAP_pattern1_tag2el_13,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pattern1_13 = {
	"pattern1",
	"pattern1",
	&asn_OP_SEQUENCE,
	asn_DEF_pattern1_tags_13,
	sizeof(asn_DEF_pattern1_tags_13)
		/sizeof(asn_DEF_pattern1_tags_13[0]) - 1, /* 1 */
	asn_DEF_pattern1_tags_13,	/* Same as above */
	sizeof(asn_DEF_pattern1_tags_13)
		/sizeof(asn_DEF_pattern1_tags_13[0]), /* 2 */
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
	asn_MBR_pattern1_13,
	2,	/* Elements count */
	&asn_SPC_pattern1_specs_13	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_csi_IM_ResourceElementPattern_3[] = {
	{ ATF_POINTER, 0, offsetof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern, choice.pattern0),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_pattern0_4,
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
		"pattern0"
		},
	{ ATF_POINTER, 0, offsetof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern, choice.pattern1),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_pattern1_13,
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
		"pattern1"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_csi_IM_ResourceElementPattern_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pattern0 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* pattern1 */
};
static asn_CHOICE_specifics_t asn_SPC_csi_IM_ResourceElementPattern_specs_3 = {
	sizeof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern),
	offsetof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern, _asn_ctx),
	offsetof(struct CSI_IM_Resource__csi_IM_ResourceElementPattern, present),
	sizeof(((struct CSI_IM_Resource__csi_IM_ResourceElementPattern *)0)->present),
	asn_MAP_csi_IM_ResourceElementPattern_tag2el_3,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_csi_IM_ResourceElementPattern_3 = {
	"csi-IM-ResourceElementPattern",
	"csi-IM-ResourceElementPattern",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_csi_IM_ResourceElementPattern_constr_3,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		CHOICE_constraint
	},
	asn_MBR_csi_IM_ResourceElementPattern_3,
	2,	/* Elements count */
	&asn_SPC_csi_IM_ResourceElementPattern_specs_3	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_CSI_IM_Resource_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_IM_Resource, csi_IM_ResourceId),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CSI_IM_ResourceId,
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
		"csi-IM-ResourceId"
		},
	{ ATF_POINTER, 3, offsetof(struct CSI_IM_Resource, csi_IM_ResourceElementPattern),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_csi_IM_ResourceElementPattern_3,
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
		"csi-IM-ResourceElementPattern"
		},
	{ ATF_POINTER, 2, offsetof(struct CSI_IM_Resource, freqBand),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CSI_FrequencyOccupation,
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
		"freqBand"
		},
	{ ATF_POINTER, 1, offsetof(struct CSI_IM_Resource, periodicityAndOffset),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_CSI_ResourcePeriodicityAndOffset,
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
		"periodicityAndOffset"
		},
};
static const int asn_MAP_CSI_IM_Resource_oms_1[] = { 1, 2, 3 };
static const ber_tlv_tag_t asn_DEF_CSI_IM_Resource_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CSI_IM_Resource_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* csi-IM-ResourceId */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* csi-IM-ResourceElementPattern */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* freqBand */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* periodicityAndOffset */
};
asn_SEQUENCE_specifics_t asn_SPC_CSI_IM_Resource_specs_1 = {
	sizeof(struct CSI_IM_Resource),
	offsetof(struct CSI_IM_Resource, _asn_ctx),
	asn_MAP_CSI_IM_Resource_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_CSI_IM_Resource_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_CSI_IM_Resource = {
	"CSI-IM-Resource",
	"CSI-IM-Resource",
	&asn_OP_SEQUENCE,
	asn_DEF_CSI_IM_Resource_tags_1,
	sizeof(asn_DEF_CSI_IM_Resource_tags_1)
		/sizeof(asn_DEF_CSI_IM_Resource_tags_1[0]), /* 1 */
	asn_DEF_CSI_IM_Resource_tags_1,	/* Same as above */
	sizeof(asn_DEF_CSI_IM_Resource_tags_1)
		/sizeof(asn_DEF_CSI_IM_Resource_tags_1[0]), /* 1 */
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
	asn_MBR_CSI_IM_Resource_1,
	4,	/* Elements count */
	&asn_SPC_CSI_IM_Resource_specs_1	/* Additional specs */
};

