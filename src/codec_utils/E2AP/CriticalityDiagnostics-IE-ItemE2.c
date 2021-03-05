/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "codeathon/E2AP.asn1"
 * 	`asn1c -D ./newE2AP -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "CriticalityDiagnostics-IE-ItemE2.h"

asn_TYPE_member_t asn_MBR_CriticalityDiagnostics_IE_ItemE2_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CriticalityDiagnostics_IE_ItemE2, iECriticalityE2),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CriticalityE2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iECriticalityE2"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CriticalityDiagnostics_IE_ItemE2, iE_ID),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolIE_IDE2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-ID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CriticalityDiagnostics_IE_ItemE2, typeOfError),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TypeOfErrorE2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"typeOfError"
		},
};
static const ber_tlv_tag_t asn_DEF_CriticalityDiagnostics_IE_ItemE2_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CriticalityDiagnostics_IE_ItemE2_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* iECriticalityE2 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* iE-ID */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* typeOfError */
};
asn_SEQUENCE_specifics_t asn_SPC_CriticalityDiagnostics_IE_ItemE2_specs_1 = {
	sizeof(struct CriticalityDiagnostics_IE_ItemE2),
	offsetof(struct CriticalityDiagnostics_IE_ItemE2, _asn_ctx),
	asn_MAP_CriticalityDiagnostics_IE_ItemE2_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_CriticalityDiagnostics_IE_ItemE2 = {
	"CriticalityDiagnostics-IE-ItemE2",
	"CriticalityDiagnostics-IE-ItemE2",
	&asn_OP_SEQUENCE,
	asn_DEF_CriticalityDiagnostics_IE_ItemE2_tags_1,
	sizeof(asn_DEF_CriticalityDiagnostics_IE_ItemE2_tags_1)
		/sizeof(asn_DEF_CriticalityDiagnostics_IE_ItemE2_tags_1[0]), /* 1 */
	asn_DEF_CriticalityDiagnostics_IE_ItemE2_tags_1,	/* Same as above */
	sizeof(asn_DEF_CriticalityDiagnostics_IE_ItemE2_tags_1)
		/sizeof(asn_DEF_CriticalityDiagnostics_IE_ItemE2_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_CriticalityDiagnostics_IE_ItemE2_1,
	3,	/* Elements count */
	&asn_SPC_CriticalityDiagnostics_IE_ItemE2_specs_1	/* Additional specs */
};

