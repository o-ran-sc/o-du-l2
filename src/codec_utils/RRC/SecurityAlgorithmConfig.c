/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SecurityAlgorithmConfig.h"

asn_TYPE_member_t asn_MBR_SecurityAlgorithmConfig_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SecurityAlgorithmConfig, cipheringAlgorithm),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CipheringAlgorithm,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cipheringAlgorithm"
		},
	{ ATF_POINTER, 1, offsetof(struct SecurityAlgorithmConfig, integrityProtAlgorithm),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IntegrityProtAlgorithm,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"integrityProtAlgorithm"
		},
};
static const int asn_MAP_SecurityAlgorithmConfig_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_SecurityAlgorithmConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SecurityAlgorithmConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* cipheringAlgorithm */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* integrityProtAlgorithm */
};
asn_SEQUENCE_specifics_t asn_SPC_SecurityAlgorithmConfig_specs_1 = {
	sizeof(struct SecurityAlgorithmConfig),
	offsetof(struct SecurityAlgorithmConfig, _asn_ctx),
	asn_MAP_SecurityAlgorithmConfig_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_SecurityAlgorithmConfig_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SecurityAlgorithmConfig = {
	"SecurityAlgorithmConfig",
	"SecurityAlgorithmConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_SecurityAlgorithmConfig_tags_1,
	sizeof(asn_DEF_SecurityAlgorithmConfig_tags_1)
		/sizeof(asn_DEF_SecurityAlgorithmConfig_tags_1[0]), /* 1 */
	asn_DEF_SecurityAlgorithmConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_SecurityAlgorithmConfig_tags_1)
		/sizeof(asn_DEF_SecurityAlgorithmConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SecurityAlgorithmConfig_1,
	2,	/* Elements count */
	&asn_SPC_SecurityAlgorithmConfig_specs_1	/* Additional specs */
};

