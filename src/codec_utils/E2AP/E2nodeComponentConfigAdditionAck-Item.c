/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "E2nodeComponentConfigAdditionAck-Item.h"

static asn_TYPE_member_t asn_MBR_E2nodeComponentConfigAdditionAck_Item_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct E2nodeComponentConfigAdditionAck_Item, e2nodeComponentInterfaceType),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_E2nodeComponentInterfaceType,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"e2nodeComponentInterfaceType"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct E2nodeComponentConfigAdditionAck_Item, e2nodeComponentID),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_E2nodeComponentID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"e2nodeComponentID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct E2nodeComponentConfigAdditionAck_Item, e2nodeComponentConfigurationAck),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_E2nodeComponentConfigurationAck,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"e2nodeComponentConfigurationAck"
		},
};
static const ber_tlv_tag_t asn_DEF_E2nodeComponentConfigAdditionAck_Item_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_E2nodeComponentConfigAdditionAck_Item_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* e2nodeComponentInterfaceType */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* e2nodeComponentID */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* e2nodeComponentConfigurationAck */
};
static asn_SEQUENCE_specifics_t asn_SPC_E2nodeComponentConfigAdditionAck_Item_specs_1 = {
	sizeof(struct E2nodeComponentConfigAdditionAck_Item),
	offsetof(struct E2nodeComponentConfigAdditionAck_Item, _asn_ctx),
	asn_MAP_E2nodeComponentConfigAdditionAck_Item_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_E2nodeComponentConfigAdditionAck_Item = {
	"E2nodeComponentConfigAdditionAck-Item",
	"E2nodeComponentConfigAdditionAck-Item",
	&asn_OP_SEQUENCE,
	asn_DEF_E2nodeComponentConfigAdditionAck_Item_tags_1,
	sizeof(asn_DEF_E2nodeComponentConfigAdditionAck_Item_tags_1)
		/sizeof(asn_DEF_E2nodeComponentConfigAdditionAck_Item_tags_1[0]), /* 1 */
	asn_DEF_E2nodeComponentConfigAdditionAck_Item_tags_1,	/* Same as above */
	sizeof(asn_DEF_E2nodeComponentConfigAdditionAck_Item_tags_1)
		/sizeof(asn_DEF_E2nodeComponentConfigAdditionAck_Item_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_E2nodeComponentConfigAdditionAck_Item_1,
	3,	/* Elements count */
	&asn_SPC_E2nodeComponentConfigAdditionAck_Item_specs_1	/* Additional specs */
};

