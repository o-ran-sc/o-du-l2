/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "MeasAndMobParameters.h"

#include "MeasAndMobParametersCommon.h"
#include "MeasAndMobParametersXDD-Diff.h"
#include "MeasAndMobParametersFRX-Diff.h"
asn_TYPE_member_t asn_MBR_MeasAndMobParameters_1[] = {
	{ ATF_POINTER, 3, offsetof(struct MeasAndMobParameters, measAndMobParametersCommon),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasAndMobParametersCommon,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"measAndMobParametersCommon"
		},
	{ ATF_POINTER, 2, offsetof(struct MeasAndMobParameters, measAndMobParametersXDD_Diff),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasAndMobParametersXDD_Diff,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"measAndMobParametersXDD-Diff"
		},
	{ ATF_POINTER, 1, offsetof(struct MeasAndMobParameters, measAndMobParametersFRX_Diff),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasAndMobParametersFRX_Diff,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"measAndMobParametersFRX-Diff"
		},
};
static const int asn_MAP_MeasAndMobParameters_oms_1[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_MeasAndMobParameters_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_MeasAndMobParameters_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* measAndMobParametersCommon */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* measAndMobParametersXDD-Diff */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* measAndMobParametersFRX-Diff */
};
asn_SEQUENCE_specifics_t asn_SPC_MeasAndMobParameters_specs_1 = {
	sizeof(struct MeasAndMobParameters),
	offsetof(struct MeasAndMobParameters, _asn_ctx),
	asn_MAP_MeasAndMobParameters_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_MeasAndMobParameters_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_MeasAndMobParameters = {
	"MeasAndMobParameters",
	"MeasAndMobParameters",
	&asn_OP_SEQUENCE,
	asn_DEF_MeasAndMobParameters_tags_1,
	sizeof(asn_DEF_MeasAndMobParameters_tags_1)
		/sizeof(asn_DEF_MeasAndMobParameters_tags_1[0]), /* 1 */
	asn_DEF_MeasAndMobParameters_tags_1,	/* Same as above */
	sizeof(asn_DEF_MeasAndMobParameters_tags_1)
		/sizeof(asn_DEF_MeasAndMobParameters_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_MeasAndMobParameters_1,
	3,	/* Elements count */
	&asn_SPC_MeasAndMobParameters_specs_1	/* Additional specs */
};

