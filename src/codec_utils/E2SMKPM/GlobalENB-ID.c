/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "codeathon/E2SM_KPM.asn1"
 * 	`asn1c -D ./codeathon/e2smkpm/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "GlobalENB-ID.h"

asn_TYPE_member_t asn_MBR_GlobalENB_ID_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct GlobalENB_ID, pLMN_Identity),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PLMN_Identity,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pLMN-Identity"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GlobalENB_ID, eNB_ID),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_ENB_ID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eNB-ID"
		},
};
static const ber_tlv_tag_t asn_DEF_GlobalENB_ID_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_GlobalENB_ID_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pLMN-Identity */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* eNB-ID */
};
asn_SEQUENCE_specifics_t asn_SPC_GlobalENB_ID_specs_1 = {
	sizeof(struct GlobalENB_ID),
	offsetof(struct GlobalENB_ID, _asn_ctx),
	asn_MAP_GlobalENB_ID_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_GlobalENB_ID = {
	"GlobalENB-ID",
	"GlobalENB-ID",
	&asn_OP_SEQUENCE,
	asn_DEF_GlobalENB_ID_tags_1,
	sizeof(asn_DEF_GlobalENB_ID_tags_1)
		/sizeof(asn_DEF_GlobalENB_ID_tags_1[0]), /* 1 */
	asn_DEF_GlobalENB_ID_tags_1,	/* Same as above */
	sizeof(asn_DEF_GlobalENB_ID_tags_1)
		/sizeof(asn_DEF_GlobalENB_ID_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_GlobalENB_ID_1,
	2,	/* Elements count */
	&asn_SPC_GlobalENB_ID_specs_1	/* Additional specs */
};

