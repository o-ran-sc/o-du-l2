/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "FeatureSetUplinkPerCC.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_maxNumberSRS_ResourcePerSet_constraint_6(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 2)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_channelBW_90mhz_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_channelBW_90mhz_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_maxNumberSRS_ResourcePerSet_constr_8 CC_NOTUSED = {
	{ 1, 1 }	/* (1..2) */,
	-1};
static asn_per_constraints_t asn_PER_memb_maxNumberSRS_ResourcePerSet_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  1,  2 }	/* (1..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_channelBW_90mhz_value2enum_4[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_channelBW_90mhz_enum2value_4[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_channelBW_90mhz_specs_4 = {
	asn_MAP_channelBW_90mhz_value2enum_4,	/* "tag" => N; sorted by tag */
	asn_MAP_channelBW_90mhz_enum2value_4,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_channelBW_90mhz_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_channelBW_90mhz_4 = {
	"channelBW-90mhz",
	"channelBW-90mhz",
	&asn_OP_NativeEnumerated,
	asn_DEF_channelBW_90mhz_tags_4,
	sizeof(asn_DEF_channelBW_90mhz_tags_4)
		/sizeof(asn_DEF_channelBW_90mhz_tags_4[0]) - 1, /* 1 */
	asn_DEF_channelBW_90mhz_tags_4,	/* Same as above */
	sizeof(asn_DEF_channelBW_90mhz_tags_4)
		/sizeof(asn_DEF_channelBW_90mhz_tags_4[0]), /* 2 */
	{ &asn_OER_type_channelBW_90mhz_constr_4, &asn_PER_type_channelBW_90mhz_constr_4, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_channelBW_90mhz_specs_4	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_mimo_CB_PUSCH_6[] = {
	{ ATF_POINTER, 1, offsetof(struct FeatureSetUplinkPerCC__mimo_CB_PUSCH, maxNumberMIMO_LayersCB_PUSCH),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MIMO_LayersUL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxNumberMIMO-LayersCB-PUSCH"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct FeatureSetUplinkPerCC__mimo_CB_PUSCH, maxNumberSRS_ResourcePerSet),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_maxNumberSRS_ResourcePerSet_constr_8, &asn_PER_memb_maxNumberSRS_ResourcePerSet_constr_8,  memb_maxNumberSRS_ResourcePerSet_constraint_6 },
		0, 0, /* No default value */
		"maxNumberSRS-ResourcePerSet"
		},
};
static const int asn_MAP_mimo_CB_PUSCH_oms_6[] = { 0 };
static const ber_tlv_tag_t asn_DEF_mimo_CB_PUSCH_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_mimo_CB_PUSCH_tag2el_6[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* maxNumberMIMO-LayersCB-PUSCH */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* maxNumberSRS-ResourcePerSet */
};
static asn_SEQUENCE_specifics_t asn_SPC_mimo_CB_PUSCH_specs_6 = {
	sizeof(struct FeatureSetUplinkPerCC__mimo_CB_PUSCH),
	offsetof(struct FeatureSetUplinkPerCC__mimo_CB_PUSCH, _asn_ctx),
	asn_MAP_mimo_CB_PUSCH_tag2el_6,
	2,	/* Count of tags in the map */
	asn_MAP_mimo_CB_PUSCH_oms_6,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_mimo_CB_PUSCH_6 = {
	"mimo-CB-PUSCH",
	"mimo-CB-PUSCH",
	&asn_OP_SEQUENCE,
	asn_DEF_mimo_CB_PUSCH_tags_6,
	sizeof(asn_DEF_mimo_CB_PUSCH_tags_6)
		/sizeof(asn_DEF_mimo_CB_PUSCH_tags_6[0]) - 1, /* 1 */
	asn_DEF_mimo_CB_PUSCH_tags_6,	/* Same as above */
	sizeof(asn_DEF_mimo_CB_PUSCH_tags_6)
		/sizeof(asn_DEF_mimo_CB_PUSCH_tags_6[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_mimo_CB_PUSCH_6,
	2,	/* Elements count */
	&asn_SPC_mimo_CB_PUSCH_specs_6	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_FeatureSetUplinkPerCC_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct FeatureSetUplinkPerCC, supportedSubcarrierSpacingUL),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SubcarrierSpacing,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"supportedSubcarrierSpacingUL"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct FeatureSetUplinkPerCC, supportedBandwidthUL),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_SupportedBandwidth,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"supportedBandwidthUL"
		},
	{ ATF_POINTER, 4, offsetof(struct FeatureSetUplinkPerCC, channelBW_90mhz),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_channelBW_90mhz_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"channelBW-90mhz"
		},
	{ ATF_POINTER, 3, offsetof(struct FeatureSetUplinkPerCC, mimo_CB_PUSCH),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		0,
		&asn_DEF_mimo_CB_PUSCH_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"mimo-CB-PUSCH"
		},
	{ ATF_POINTER, 2, offsetof(struct FeatureSetUplinkPerCC, maxNumberMIMO_LayersNonCB_PUSCH),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MIMO_LayersUL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxNumberMIMO-LayersNonCB-PUSCH"
		},
	{ ATF_POINTER, 1, offsetof(struct FeatureSetUplinkPerCC, supportedModulationOrderUL),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ModulationOrder,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"supportedModulationOrderUL"
		},
};
static const int asn_MAP_FeatureSetUplinkPerCC_oms_1[] = { 2, 3, 4, 5 };
static const ber_tlv_tag_t asn_DEF_FeatureSetUplinkPerCC_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_FeatureSetUplinkPerCC_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* supportedSubcarrierSpacingUL */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* supportedBandwidthUL */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* channelBW-90mhz */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* mimo-CB-PUSCH */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* maxNumberMIMO-LayersNonCB-PUSCH */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* supportedModulationOrderUL */
};
asn_SEQUENCE_specifics_t asn_SPC_FeatureSetUplinkPerCC_specs_1 = {
	sizeof(struct FeatureSetUplinkPerCC),
	offsetof(struct FeatureSetUplinkPerCC, _asn_ctx),
	asn_MAP_FeatureSetUplinkPerCC_tag2el_1,
	6,	/* Count of tags in the map */
	asn_MAP_FeatureSetUplinkPerCC_oms_1,	/* Optional members */
	4, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_FeatureSetUplinkPerCC = {
	"FeatureSetUplinkPerCC",
	"FeatureSetUplinkPerCC",
	&asn_OP_SEQUENCE,
	asn_DEF_FeatureSetUplinkPerCC_tags_1,
	sizeof(asn_DEF_FeatureSetUplinkPerCC_tags_1)
		/sizeof(asn_DEF_FeatureSetUplinkPerCC_tags_1[0]), /* 1 */
	asn_DEF_FeatureSetUplinkPerCC_tags_1,	/* Same as above */
	sizeof(asn_DEF_FeatureSetUplinkPerCC_tags_1)
		/sizeof(asn_DEF_FeatureSetUplinkPerCC_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_FeatureSetUplinkPerCC_1,
	6,	/* Elements count */
	&asn_SPC_FeatureSetUplinkPerCC_specs_1	/* Additional specs */
};

