/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "PacketErrorRate.h"

#include "ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_PacketErrorRate_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PacketErrorRate, pER_Scalar),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PER_Scalar,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pER-Scalar"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PacketErrorRate, pER_Exponent),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PER_Exponent,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pER-Exponent"
		},
	{ ATF_POINTER, 1, offsetof(struct PacketErrorRate, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_4624P68,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_PacketErrorRate_oms_1[] = { 2 };
static const ber_tlv_tag_t asn_DEF_PacketErrorRate_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PacketErrorRate_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pER-Scalar */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* pER-Exponent */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_PacketErrorRate_specs_1 = {
	sizeof(struct PacketErrorRate),
	offsetof(struct PacketErrorRate, _asn_ctx),
	asn_MAP_PacketErrorRate_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_PacketErrorRate_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_PacketErrorRate = {
	"PacketErrorRate",
	"PacketErrorRate",
	&asn_OP_SEQUENCE,
	asn_DEF_PacketErrorRate_tags_1,
	sizeof(asn_DEF_PacketErrorRate_tags_1)
		/sizeof(asn_DEF_PacketErrorRate_tags_1[0]), /* 1 */
	asn_DEF_PacketErrorRate_tags_1,	/* Same as above */
	sizeof(asn_DEF_PacketErrorRate_tags_1)
		/sizeof(asn_DEF_PacketErrorRate_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_PacketErrorRate_1,
	3,	/* Elements count */
	&asn_SPC_PacketErrorRate_specs_1	/* Additional specs */
};

