/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "UL-DCCH-Message.h"

static asn_TYPE_member_t asn_MBR_UL_DCCH_Message_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_Message, message),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_UL_DCCH_MessageType,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"message"
		},
};
static const ber_tlv_tag_t asn_DEF_UL_DCCH_Message_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UL_DCCH_Message_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* message */
};
static asn_SEQUENCE_specifics_t asn_SPC_UL_DCCH_Message_specs_1 = {
	sizeof(struct UL_DCCH_Message),
	offsetof(struct UL_DCCH_Message, _asn_ctx),
	asn_MAP_UL_DCCH_Message_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_UL_DCCH_Message = {
	"UL-DCCH-Message",
	"UL-DCCH-Message",
	&asn_OP_SEQUENCE,
	asn_DEF_UL_DCCH_Message_tags_1,
	sizeof(asn_DEF_UL_DCCH_Message_tags_1)
		/sizeof(asn_DEF_UL_DCCH_Message_tags_1[0]), /* 1 */
	asn_DEF_UL_DCCH_Message_tags_1,	/* Same as above */
	sizeof(asn_DEF_UL_DCCH_Message_tags_1)
		/sizeof(asn_DEF_UL_DCCH_Message_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_UL_DCCH_Message_1,
	1,	/* Elements count */
	&asn_SPC_UL_DCCH_Message_specs_1	/* Additional specs */
};

