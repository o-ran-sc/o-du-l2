/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "DRBs-SetupMod-Item.h"

#include "ProtocolExtensionContainer.h"
static asn_TYPE_member_t asn_MBR_DRBs_SetupMod_Item_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DRBs_SetupMod_Item, dRBID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DRBID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dRBID"
		},
	{ ATF_POINTER, 1, offsetof(struct DRBs_SetupMod_Item, lCID),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LCID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"lCID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DRBs_SetupMod_Item, dLUPTNLInformation_ToBeSetup_List),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DLUPTNLInformation_ToBeSetup_List,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dLUPTNLInformation-ToBeSetup-List"
		},
	{ ATF_POINTER, 1, offsetof(struct DRBs_SetupMod_Item, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_4624P30,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_DRBs_SetupMod_Item_oms_1[] = { 1, 3 };
static const ber_tlv_tag_t asn_DEF_DRBs_SetupMod_Item_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DRBs_SetupMod_Item_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* dRBID */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* lCID */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* dLUPTNLInformation-ToBeSetup-List */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* iE-Extensions */
};
static asn_SEQUENCE_specifics_t asn_SPC_DRBs_SetupMod_Item_specs_1 = {
	sizeof(struct DRBs_SetupMod_Item),
	offsetof(struct DRBs_SetupMod_Item, _asn_ctx),
	asn_MAP_DRBs_SetupMod_Item_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_DRBs_SetupMod_Item_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_DRBs_SetupMod_Item = {
	"DRBs-SetupMod-Item",
	"DRBs-SetupMod-Item",
	&asn_OP_SEQUENCE,
	asn_DEF_DRBs_SetupMod_Item_tags_1,
	sizeof(asn_DEF_DRBs_SetupMod_Item_tags_1)
		/sizeof(asn_DEF_DRBs_SetupMod_Item_tags_1[0]), /* 1 */
	asn_DEF_DRBs_SetupMod_Item_tags_1,	/* Same as above */
	sizeof(asn_DEF_DRBs_SetupMod_Item_tags_1)
		/sizeof(asn_DEF_DRBs_SetupMod_Item_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_DRBs_SetupMod_Item_1,
	4,	/* Elements count */
	&asn_SPC_DRBs_SetupMod_Item_specs_1	/* Additional specs */
};

