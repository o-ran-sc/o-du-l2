/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "codeathon/E2SM_KPM.asn1"
 * 	`asn1c -D ./codeathon/e2smkpm/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "RIC-ReportStyle-List.h"

asn_TYPE_member_t asn_MBR_RIC_ReportStyle_List_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct RIC_ReportStyle_List, ric_ReportStyle_Type),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RIC_Style_Type,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ric-ReportStyle-Type"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RIC_ReportStyle_List, ric_ReportStyle_Name),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RIC_Style_Name,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ric-ReportStyle-Name"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RIC_ReportStyle_List, ric_IndicationHeaderFormat_Type),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RIC_Format_Type,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ric-IndicationHeaderFormat-Type"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RIC_ReportStyle_List, ric_IndicationMessageFormat_Type),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RIC_Format_Type,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ric-IndicationMessageFormat-Type"
		},
};
static const ber_tlv_tag_t asn_DEF_RIC_ReportStyle_List_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_RIC_ReportStyle_List_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ric-ReportStyle-Type */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* ric-ReportStyle-Name */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* ric-IndicationHeaderFormat-Type */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* ric-IndicationMessageFormat-Type */
};
asn_SEQUENCE_specifics_t asn_SPC_RIC_ReportStyle_List_specs_1 = {
	sizeof(struct RIC_ReportStyle_List),
	offsetof(struct RIC_ReportStyle_List, _asn_ctx),
	asn_MAP_RIC_ReportStyle_List_tag2el_1,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_RIC_ReportStyle_List = {
	"RIC-ReportStyle-List",
	"RIC-ReportStyle-List",
	&asn_OP_SEQUENCE,
	asn_DEF_RIC_ReportStyle_List_tags_1,
	sizeof(asn_DEF_RIC_ReportStyle_List_tags_1)
		/sizeof(asn_DEF_RIC_ReportStyle_List_tags_1[0]), /* 1 */
	asn_DEF_RIC_ReportStyle_List_tags_1,	/* Same as above */
	sizeof(asn_DEF_RIC_ReportStyle_List_tags_1)
		/sizeof(asn_DEF_RIC_ReportStyle_List_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_RIC_ReportStyle_List_1,
	4,	/* Elements count */
	&asn_SPC_RIC_ReportStyle_List_specs_1	/* Additional specs */
};

