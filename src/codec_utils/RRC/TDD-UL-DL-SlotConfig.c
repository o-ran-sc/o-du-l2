/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "TDD-UL-DL-SlotConfig.h"

static int
memb_nrofDownlinkSymbols_constraint_6(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 13)) {
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
memb_nrofUplinkSymbols_constraint_6(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 13)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_nrofDownlinkSymbols_constr_7 CC_NOTUSED = {
	{ 1, 1 }	/* (1..13) */,
	-1};
static asn_per_constraints_t asn_PER_memb_nrofDownlinkSymbols_constr_7 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  1,  13 }	/* (1..13) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_nrofUplinkSymbols_constr_8 CC_NOTUSED = {
	{ 1, 1 }	/* (1..13) */,
	-1};
static asn_per_constraints_t asn_PER_memb_nrofUplinkSymbols_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  1,  13 }	/* (1..13) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_symbols_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_symbols_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_explicit_6[] = {
	{ ATF_POINTER, 2, offsetof(struct TDD_UL_DL_SlotConfig__symbols__explicit, nrofDownlinkSymbols),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_nrofDownlinkSymbols_constr_7, &asn_PER_memb_nrofDownlinkSymbols_constr_7,  memb_nrofDownlinkSymbols_constraint_6 },
		0, 0, /* No default value */
		"nrofDownlinkSymbols"
		},
	{ ATF_POINTER, 1, offsetof(struct TDD_UL_DL_SlotConfig__symbols__explicit, nrofUplinkSymbols),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_nrofUplinkSymbols_constr_8, &asn_PER_memb_nrofUplinkSymbols_constr_8,  memb_nrofUplinkSymbols_constraint_6 },
		0, 0, /* No default value */
		"nrofUplinkSymbols"
		},
};
static const int asn_MAP_explicit_oms_6[] = { 0, 1 };
static const ber_tlv_tag_t asn_DEF_explicit_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_explicit_tag2el_6[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* nrofDownlinkSymbols */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* nrofUplinkSymbols */
};
static asn_SEQUENCE_specifics_t asn_SPC_explicit_specs_6 = {
	sizeof(struct TDD_UL_DL_SlotConfig__symbols__explicit),
	offsetof(struct TDD_UL_DL_SlotConfig__symbols__explicit, _asn_ctx),
	asn_MAP_explicit_tag2el_6,
	2,	/* Count of tags in the map */
	asn_MAP_explicit_oms_6,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_explicit_6 = {
	"explicit",
	"explicit",
	&asn_OP_SEQUENCE,
	asn_DEF_explicit_tags_6,
	sizeof(asn_DEF_explicit_tags_6)
		/sizeof(asn_DEF_explicit_tags_6[0]) - 1, /* 1 */
	asn_DEF_explicit_tags_6,	/* Same as above */
	sizeof(asn_DEF_explicit_tags_6)
		/sizeof(asn_DEF_explicit_tags_6[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_explicit_6,
	2,	/* Elements count */
	&asn_SPC_explicit_specs_6	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_symbols_3[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TDD_UL_DL_SlotConfig__symbols, choice.allDownlink),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"allDownlink"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TDD_UL_DL_SlotConfig__symbols, choice.allUplink),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"allUplink"
		},
	{ ATF_POINTER, 0, offsetof(struct TDD_UL_DL_SlotConfig__symbols, choice.Explicit),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_explicit_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"explicit"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_symbols_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* allDownlink */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* allUplink */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* explicit */
};
static asn_CHOICE_specifics_t asn_SPC_symbols_specs_3 = {
	sizeof(struct TDD_UL_DL_SlotConfig__symbols),
	offsetof(struct TDD_UL_DL_SlotConfig__symbols, _asn_ctx),
	offsetof(struct TDD_UL_DL_SlotConfig__symbols, present),
	sizeof(((struct TDD_UL_DL_SlotConfig__symbols *)0)->present),
	asn_MAP_symbols_tag2el_3,
	3,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_symbols_3 = {
	"symbols",
	"symbols",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_symbols_constr_3, &asn_PER_type_symbols_constr_3, CHOICE_constraint },
	asn_MBR_symbols_3,
	3,	/* Elements count */
	&asn_SPC_symbols_specs_3	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_TDD_UL_DL_SlotConfig_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TDD_UL_DL_SlotConfig, slotIndex),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TDD_UL_DL_SlotIndex,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"slotIndex"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TDD_UL_DL_SlotConfig, symbols),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_symbols_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"symbols"
		},
};
static const ber_tlv_tag_t asn_DEF_TDD_UL_DL_SlotConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_TDD_UL_DL_SlotConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* slotIndex */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* symbols */
};
asn_SEQUENCE_specifics_t asn_SPC_TDD_UL_DL_SlotConfig_specs_1 = {
	sizeof(struct TDD_UL_DL_SlotConfig),
	offsetof(struct TDD_UL_DL_SlotConfig, _asn_ctx),
	asn_MAP_TDD_UL_DL_SlotConfig_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_TDD_UL_DL_SlotConfig = {
	"TDD-UL-DL-SlotConfig",
	"TDD-UL-DL-SlotConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_TDD_UL_DL_SlotConfig_tags_1,
	sizeof(asn_DEF_TDD_UL_DL_SlotConfig_tags_1)
		/sizeof(asn_DEF_TDD_UL_DL_SlotConfig_tags_1[0]), /* 1 */
	asn_DEF_TDD_UL_DL_SlotConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_TDD_UL_DL_SlotConfig_tags_1)
		/sizeof(asn_DEF_TDD_UL_DL_SlotConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_TDD_UL_DL_SlotConfig_1,
	2,	/* Elements count */
	&asn_SPC_TDD_UL_DL_SlotConfig_specs_1	/* Additional specs */
};

