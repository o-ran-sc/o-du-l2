/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "HandoverPreparationInformation-IEs.h"

#include "AS-Config.h"
#include "RRM-Config.h"
#include "AS-Context.h"
static const ber_tlv_tag_t asn_DEF_nonCriticalExtension_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SEQUENCE_specifics_t asn_SPC_nonCriticalExtension_specs_6 = {
	sizeof(struct HandoverPreparationInformation_IEs__nonCriticalExtension),
	offsetof(struct HandoverPreparationInformation_IEs__nonCriticalExtension, _asn_ctx),
	0,	/* No top level tags */
	0,	/* No tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_nonCriticalExtension_6 = {
	"nonCriticalExtension",
	"nonCriticalExtension",
	&asn_OP_SEQUENCE,
	asn_DEF_nonCriticalExtension_tags_6,
	sizeof(asn_DEF_nonCriticalExtension_tags_6)
		/sizeof(asn_DEF_nonCriticalExtension_tags_6[0]) - 1, /* 1 */
	asn_DEF_nonCriticalExtension_tags_6,	/* Same as above */
	sizeof(asn_DEF_nonCriticalExtension_tags_6)
		/sizeof(asn_DEF_nonCriticalExtension_tags_6[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	0, 0,	/* No members */
	&asn_SPC_nonCriticalExtension_specs_6	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_HandoverPreparationInformation_IEs_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct HandoverPreparationInformation_IEs, ue_CapabilityRAT_List),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UE_CapabilityRAT_ContainerListRRC,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ue-CapabilityRAT-List"
		},
	{ ATF_POINTER, 4, offsetof(struct HandoverPreparationInformation_IEs, sourceConfig),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AS_Config,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sourceConfig"
		},
	{ ATF_POINTER, 3, offsetof(struct HandoverPreparationInformation_IEs, rrm_Config),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RRM_Config,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rrm-Config"
		},
	{ ATF_POINTER, 2, offsetof(struct HandoverPreparationInformation_IEs, as_Context),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AS_Context,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"as-Context"
		},
	{ ATF_POINTER, 1, offsetof(struct HandoverPreparationInformation_IEs, nonCriticalExtension),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		0,
		&asn_DEF_nonCriticalExtension_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"nonCriticalExtension"
		},
};
static const int asn_MAP_HandoverPreparationInformation_IEs_oms_1[] = { 1, 2, 3, 4 };
static const ber_tlv_tag_t asn_DEF_HandoverPreparationInformation_IEs_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_HandoverPreparationInformation_IEs_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ue-CapabilityRAT-List */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* sourceConfig */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* rrm-Config */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* as-Context */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* nonCriticalExtension */
};
asn_SEQUENCE_specifics_t asn_SPC_HandoverPreparationInformation_IEs_specs_1 = {
	sizeof(struct HandoverPreparationInformation_IEs),
	offsetof(struct HandoverPreparationInformation_IEs, _asn_ctx),
	asn_MAP_HandoverPreparationInformation_IEs_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_HandoverPreparationInformation_IEs_oms_1,	/* Optional members */
	4, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_HandoverPreparationInformation_IEs = {
	"HandoverPreparationInformation-IEs",
	"HandoverPreparationInformation-IEs",
	&asn_OP_SEQUENCE,
	asn_DEF_HandoverPreparationInformation_IEs_tags_1,
	sizeof(asn_DEF_HandoverPreparationInformation_IEs_tags_1)
		/sizeof(asn_DEF_HandoverPreparationInformation_IEs_tags_1[0]), /* 1 */
	asn_DEF_HandoverPreparationInformation_IEs_tags_1,	/* Same as above */
	sizeof(asn_DEF_HandoverPreparationInformation_IEs_tags_1)
		/sizeof(asn_DEF_HandoverPreparationInformation_IEs_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_HandoverPreparationInformation_IEs_1,
	5,	/* Elements count */
	&asn_SPC_HandoverPreparationInformation_IEs_specs_1	/* Additional specs */
};

