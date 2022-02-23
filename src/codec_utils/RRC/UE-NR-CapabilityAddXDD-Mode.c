/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "UE-NR-CapabilityAddXDD-Mode.h"

#include "Phy-ParametersXDD-Diff.h"
#include "MAC-ParametersXDD-Diff.h"
#include "MeasAndMobParametersXDD-Diff.h"
asn_TYPE_member_t asn_MBR_UE_NR_CapabilityAddXDD_Mode_1[] = {
	{ ATF_POINTER, 3, offsetof(struct UE_NR_CapabilityAddXDD_Mode, phy_ParametersXDD_Diff),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Phy_ParametersXDD_Diff,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"phy-ParametersXDD-Diff"
		},
	{ ATF_POINTER, 2, offsetof(struct UE_NR_CapabilityAddXDD_Mode, mac_ParametersXDD_Diff),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MAC_ParametersXDD_Diff,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"mac-ParametersXDD-Diff"
		},
	{ ATF_POINTER, 1, offsetof(struct UE_NR_CapabilityAddXDD_Mode, measAndMobParametersXDD_Diff),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasAndMobParametersXDD_Diff,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"measAndMobParametersXDD-Diff"
		},
};
static const int asn_MAP_UE_NR_CapabilityAddXDD_Mode_oms_1[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_UE_NR_CapabilityAddXDD_Mode_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UE_NR_CapabilityAddXDD_Mode_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* phy-ParametersXDD-Diff */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* mac-ParametersXDD-Diff */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* measAndMobParametersXDD-Diff */
};
asn_SEQUENCE_specifics_t asn_SPC_UE_NR_CapabilityAddXDD_Mode_specs_1 = {
	sizeof(struct UE_NR_CapabilityAddXDD_Mode),
	offsetof(struct UE_NR_CapabilityAddXDD_Mode, _asn_ctx),
	asn_MAP_UE_NR_CapabilityAddXDD_Mode_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_UE_NR_CapabilityAddXDD_Mode_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_UE_NR_CapabilityAddXDD_Mode = {
	"UE-NR-CapabilityAddXDD-Mode",
	"UE-NR-CapabilityAddXDD-Mode",
	&asn_OP_SEQUENCE,
	asn_DEF_UE_NR_CapabilityAddXDD_Mode_tags_1,
	sizeof(asn_DEF_UE_NR_CapabilityAddXDD_Mode_tags_1)
		/sizeof(asn_DEF_UE_NR_CapabilityAddXDD_Mode_tags_1[0]), /* 1 */
	asn_DEF_UE_NR_CapabilityAddXDD_Mode_tags_1,	/* Same as above */
	sizeof(asn_DEF_UE_NR_CapabilityAddXDD_Mode_tags_1)
		/sizeof(asn_DEF_UE_NR_CapabilityAddXDD_Mode_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_UE_NR_CapabilityAddXDD_Mode_1,
	3,	/* Elements count */
	&asn_SPC_UE_NR_CapabilityAddXDD_Mode_specs_1	/* Additional specs */
};

