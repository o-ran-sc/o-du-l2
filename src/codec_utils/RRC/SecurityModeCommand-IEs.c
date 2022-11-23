/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "SecurityModeCommand-IEs.h"

static const ber_tlv_tag_t asn_DEF_nonCriticalExtension_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SEQUENCE_specifics_t asn_SPC_nonCriticalExtension_specs_4 = {
	sizeof(struct SecurityModeCommand_IEs__nonCriticalExtension),
	offsetof(struct SecurityModeCommand_IEs__nonCriticalExtension, _asn_ctx),
	0,	/* No top level tags */
	0,	/* No tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_nonCriticalExtension_4 = {
	"nonCriticalExtension",
	"nonCriticalExtension",
	&asn_OP_SEQUENCE,
	asn_DEF_nonCriticalExtension_tags_4,
	sizeof(asn_DEF_nonCriticalExtension_tags_4)
		/sizeof(asn_DEF_nonCriticalExtension_tags_4[0]) - 1, /* 1 */
	asn_DEF_nonCriticalExtension_tags_4,	/* Same as above */
	sizeof(asn_DEF_nonCriticalExtension_tags_4)
		/sizeof(asn_DEF_nonCriticalExtension_tags_4[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	0, 0,	/* No members */
	&asn_SPC_nonCriticalExtension_specs_4	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SecurityModeCommand_IEs_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SecurityModeCommand_IEs, securityConfigSMC),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SecurityConfigSMC,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"securityConfigSMC"
		},
	{ ATF_POINTER, 2, offsetof(struct SecurityModeCommand_IEs, lateNonCriticalExtension),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OCTET_STRING,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"lateNonCriticalExtension"
		},
	{ ATF_POINTER, 1, offsetof(struct SecurityModeCommand_IEs, nonCriticalExtension),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_nonCriticalExtension_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"nonCriticalExtension"
		},
};
static const int asn_MAP_SecurityModeCommand_IEs_oms_1[] = { 1, 2 };
static const ber_tlv_tag_t asn_DEF_SecurityModeCommand_IEs_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SecurityModeCommand_IEs_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* securityConfigSMC */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* lateNonCriticalExtension */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* nonCriticalExtension */
};
asn_SEQUENCE_specifics_t asn_SPC_SecurityModeCommand_IEs_specs_1 = {
	sizeof(struct SecurityModeCommand_IEs),
	offsetof(struct SecurityModeCommand_IEs, _asn_ctx),
	asn_MAP_SecurityModeCommand_IEs_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_SecurityModeCommand_IEs_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SecurityModeCommand_IEs = {
	"SecurityModeCommand-IEs",
	"SecurityModeCommand-IEs",
	&asn_OP_SEQUENCE,
	asn_DEF_SecurityModeCommand_IEs_tags_1,
	sizeof(asn_DEF_SecurityModeCommand_IEs_tags_1)
		/sizeof(asn_DEF_SecurityModeCommand_IEs_tags_1[0]), /* 1 */
	asn_DEF_SecurityModeCommand_IEs_tags_1,	/* Same as above */
	sizeof(asn_DEF_SecurityModeCommand_IEs_tags_1)
		/sizeof(asn_DEF_SecurityModeCommand_IEs_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SecurityModeCommand_IEs_1,
	3,	/* Elements count */
	&asn_SPC_SecurityModeCommand_IEs_specs_1	/* Additional specs */
};

