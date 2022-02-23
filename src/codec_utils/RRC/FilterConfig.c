/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "FilterConfig.h"

static int asn_DFL_2_cmp_4(const void *sptr) {
	const FilterCoefficient_t *st = sptr;
	
	if(!st) {
		return -1; /* No value is not a default value */
	}
	
	/* Test default value 4 */
	return (*st != 4);
}
static int asn_DFL_2_set_4(void **sptr) {
	FilterCoefficient_t *st = *sptr;
	
	if(!st) {
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	/* Install default value 4 */
	*st = 4;
	return 0;
}
static int asn_DFL_3_cmp_4(const void *sptr) {
	const FilterCoefficient_t *st = sptr;
	
	if(!st) {
		return -1; /* No value is not a default value */
	}
	
	/* Test default value 4 */
	return (*st != 4);
}
static int asn_DFL_3_set_4(void **sptr) {
	FilterCoefficient_t *st = *sptr;
	
	if(!st) {
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	/* Install default value 4 */
	*st = 4;
	return 0;
}
static int asn_DFL_4_cmp_4(const void *sptr) {
	const FilterCoefficient_t *st = sptr;
	
	if(!st) {
		return -1; /* No value is not a default value */
	}
	
	/* Test default value 4 */
	return (*st != 4);
}
static int asn_DFL_4_set_4(void **sptr) {
	FilterCoefficient_t *st = *sptr;
	
	if(!st) {
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	/* Install default value 4 */
	*st = 4;
	return 0;
}
asn_TYPE_member_t asn_MBR_FilterConfig_1[] = {
	{ ATF_POINTER, 3, offsetof(struct FilterConfig, filterCoefficientRSRP),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FilterCoefficient,
		0,
		{ 0, 0, 0 },
		&asn_DFL_2_cmp_4,	/* Compare DEFAULT 4 */
		&asn_DFL_2_set_4,	/* Set DEFAULT 4 */
		"filterCoefficientRSRP"
		},
	{ ATF_POINTER, 2, offsetof(struct FilterConfig, filterCoefficientRSRQ),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FilterCoefficient,
		0,
		{ 0, 0, 0 },
		&asn_DFL_3_cmp_4,	/* Compare DEFAULT 4 */
		&asn_DFL_3_set_4,	/* Set DEFAULT 4 */
		"filterCoefficientRSRQ"
		},
	{ ATF_POINTER, 1, offsetof(struct FilterConfig, filterCoefficientRS_SINR),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FilterCoefficient,
		0,
		{ 0, 0, 0 },
		&asn_DFL_4_cmp_4,	/* Compare DEFAULT 4 */
		&asn_DFL_4_set_4,	/* Set DEFAULT 4 */
		"filterCoefficientRS-SINR"
		},
};
static const int asn_MAP_FilterConfig_oms_1[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_FilterConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_FilterConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* filterCoefficientRSRP */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* filterCoefficientRSRQ */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* filterCoefficientRS-SINR */
};
asn_SEQUENCE_specifics_t asn_SPC_FilterConfig_specs_1 = {
	sizeof(struct FilterConfig),
	offsetof(struct FilterConfig, _asn_ctx),
	asn_MAP_FilterConfig_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_FilterConfig_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_FilterConfig = {
	"FilterConfig",
	"FilterConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_FilterConfig_tags_1,
	sizeof(asn_DEF_FilterConfig_tags_1)
		/sizeof(asn_DEF_FilterConfig_tags_1[0]), /* 1 */
	asn_DEF_FilterConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_FilterConfig_tags_1)
		/sizeof(asn_DEF_FilterConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_FilterConfig_1,
	3,	/* Elements count */
	&asn_SPC_FilterConfig_specs_1	/* Additional specs */
};

