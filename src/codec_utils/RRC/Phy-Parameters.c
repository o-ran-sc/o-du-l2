/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "Phy-Parameters.h"

#include "Phy-ParametersCommon.h"
#include "Phy-ParametersXDD-Diff.h"
#include "Phy-ParametersFRX-Diff.h"
#include "Phy-ParametersFR1.h"
#include "Phy-ParametersFR2.h"
asn_TYPE_member_t asn_MBR_Phy_Parameters_1[] = {
	{ ATF_POINTER, 5, offsetof(struct Phy_Parameters, phy_ParametersCommon),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Phy_ParametersCommon,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"phy-ParametersCommon"
		},
	{ ATF_POINTER, 4, offsetof(struct Phy_Parameters, phy_ParametersXDD_Diff),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Phy_ParametersXDD_Diff,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"phy-ParametersXDD-Diff"
		},
	{ ATF_POINTER, 3, offsetof(struct Phy_Parameters, phy_ParametersFRX_Diff),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Phy_ParametersFRX_Diff,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"phy-ParametersFRX-Diff"
		},
	{ ATF_POINTER, 2, offsetof(struct Phy_Parameters, phy_ParametersFR1),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Phy_ParametersFR1,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"phy-ParametersFR1"
		},
	{ ATF_POINTER, 1, offsetof(struct Phy_Parameters, phy_ParametersFR2),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Phy_ParametersFR2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"phy-ParametersFR2"
		},
};
static const int asn_MAP_Phy_Parameters_oms_1[] = { 0, 1, 2, 3, 4 };
static const ber_tlv_tag_t asn_DEF_Phy_Parameters_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Phy_Parameters_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* phy-ParametersCommon */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* phy-ParametersXDD-Diff */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* phy-ParametersFRX-Diff */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* phy-ParametersFR1 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* phy-ParametersFR2 */
};
asn_SEQUENCE_specifics_t asn_SPC_Phy_Parameters_specs_1 = {
	sizeof(struct Phy_Parameters),
	offsetof(struct Phy_Parameters, _asn_ctx),
	asn_MAP_Phy_Parameters_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_Phy_Parameters_oms_1,	/* Optional members */
	5, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Phy_Parameters = {
	"Phy-Parameters",
	"Phy-Parameters",
	&asn_OP_SEQUENCE,
	asn_DEF_Phy_Parameters_tags_1,
	sizeof(asn_DEF_Phy_Parameters_tags_1)
		/sizeof(asn_DEF_Phy_Parameters_tags_1[0]), /* 1 */
	asn_DEF_Phy_Parameters_tags_1,	/* Same as above */
	sizeof(asn_DEF_Phy_Parameters_tags_1)
		/sizeof(asn_DEF_Phy_Parameters_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_Phy_Parameters_1,
	5,	/* Elements count */
	&asn_SPC_Phy_Parameters_specs_1	/* Additional specs */
};

