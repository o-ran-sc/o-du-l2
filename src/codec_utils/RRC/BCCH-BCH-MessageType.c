/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "BCCH-BCH-MessageType.h"

#include "MIB.h"
static asn_oer_constraints_t asn_OER_type_BCCH_BCH_MessageType_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_BCCH_BCH_MessageType_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const ber_tlv_tag_t asn_DEF_messageClassExtension_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SEQUENCE_specifics_t asn_SPC_messageClassExtension_specs_3 = {
	sizeof(struct BCCH_BCH_MessageType__messageClassExtension),
	offsetof(struct BCCH_BCH_MessageType__messageClassExtension, _asn_ctx),
	0,	/* No top level tags */
	0,	/* No tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_messageClassExtension_3 = {
	"messageClassExtension",
	"messageClassExtension",
	&asn_OP_SEQUENCE,
	asn_DEF_messageClassExtension_tags_3,
	sizeof(asn_DEF_messageClassExtension_tags_3)
		/sizeof(asn_DEF_messageClassExtension_tags_3[0]) - 1, /* 1 */
	asn_DEF_messageClassExtension_tags_3,	/* Same as above */
	sizeof(asn_DEF_messageClassExtension_tags_3)
		/sizeof(asn_DEF_messageClassExtension_tags_3[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	0, 0,	/* No members */
	&asn_SPC_messageClassExtension_specs_3	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_BCCH_BCH_MessageType_1[] = {
	{ ATF_POINTER, 0, offsetof(struct BCCH_BCH_MessageType, choice.mib),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MIB,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"mib"
		},
	{ ATF_POINTER, 0, offsetof(struct BCCH_BCH_MessageType, choice.messageClassExtension),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_messageClassExtension_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"messageClassExtension"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_BCCH_BCH_MessageType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* mib */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* messageClassExtension */
};
asn_CHOICE_specifics_t asn_SPC_BCCH_BCH_MessageType_specs_1 = {
	sizeof(struct BCCH_BCH_MessageType),
	offsetof(struct BCCH_BCH_MessageType, _asn_ctx),
	offsetof(struct BCCH_BCH_MessageType, present),
	sizeof(((struct BCCH_BCH_MessageType *)0)->present),
	asn_MAP_BCCH_BCH_MessageType_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_BCCH_BCH_MessageType = {
	"BCCH-BCH-MessageType",
	"BCCH-BCH-MessageType",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_BCCH_BCH_MessageType_constr_1, &asn_PER_type_BCCH_BCH_MessageType_constr_1, CHOICE_constraint },
	asn_MBR_BCCH_BCH_MessageType_1,
	2,	/* Elements count */
	&asn_SPC_BCCH_BCH_MessageType_specs_1	/* Additional specs */
};

