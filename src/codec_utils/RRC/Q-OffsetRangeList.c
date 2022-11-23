/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "Q-OffsetRangeList.h"

static int asn_DFL_2_cmp_15(const void *sptr) {
	const Q_OffsetRange_t *st = sptr;
	
	if(!st) {
		return -1; /* No value is not a default value */
	}
	
	/* Test default value 15 */
	return (*st != 15);
}
static int asn_DFL_2_set_15(void **sptr) {
	Q_OffsetRange_t *st = *sptr;
	
	if(!st) {
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	/* Install default value 15 */
	*st = 15;
	return 0;
}
static int asn_DFL_3_cmp_15(const void *sptr) {
	const Q_OffsetRange_t *st = sptr;
	
	if(!st) {
		return -1; /* No value is not a default value */
	}
	
	/* Test default value 15 */
	return (*st != 15);
}
static int asn_DFL_3_set_15(void **sptr) {
	Q_OffsetRange_t *st = *sptr;
	
	if(!st) {
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	/* Install default value 15 */
	*st = 15;
	return 0;
}
static int asn_DFL_4_cmp_15(const void *sptr) {
	const Q_OffsetRange_t *st = sptr;
	
	if(!st) {
		return -1; /* No value is not a default value */
	}
	
	/* Test default value 15 */
	return (*st != 15);
}
static int asn_DFL_4_set_15(void **sptr) {
	Q_OffsetRange_t *st = *sptr;
	
	if(!st) {
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	/* Install default value 15 */
	*st = 15;
	return 0;
}
static int asn_DFL_5_cmp_15(const void *sptr) {
	const Q_OffsetRange_t *st = sptr;
	
	if(!st) {
		return -1; /* No value is not a default value */
	}
	
	/* Test default value 15 */
	return (*st != 15);
}
static int asn_DFL_5_set_15(void **sptr) {
	Q_OffsetRange_t *st = *sptr;
	
	if(!st) {
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	/* Install default value 15 */
	*st = 15;
	return 0;
}
static int asn_DFL_6_cmp_15(const void *sptr) {
	const Q_OffsetRange_t *st = sptr;
	
	if(!st) {
		return -1; /* No value is not a default value */
	}
	
	/* Test default value 15 */
	return (*st != 15);
}
static int asn_DFL_6_set_15(void **sptr) {
	Q_OffsetRange_t *st = *sptr;
	
	if(!st) {
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	/* Install default value 15 */
	*st = 15;
	return 0;
}
static int asn_DFL_7_cmp_15(const void *sptr) {
	const Q_OffsetRange_t *st = sptr;
	
	if(!st) {
		return -1; /* No value is not a default value */
	}
	
	/* Test default value 15 */
	return (*st != 15);
}
static int asn_DFL_7_set_15(void **sptr) {
	Q_OffsetRange_t *st = *sptr;
	
	if(!st) {
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	/* Install default value 15 */
	*st = 15;
	return 0;
}
asn_TYPE_member_t asn_MBR_Q_OffsetRangeList_1[] = {
	{ ATF_POINTER, 6, offsetof(struct Q_OffsetRangeList, rsrpOffsetSSB),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Q_OffsetRange,
		0,
		{ 0, 0, 0 },
		&asn_DFL_2_cmp_15,	/* Compare DEFAULT 15 */
		&asn_DFL_2_set_15,	/* Set DEFAULT 15 */
		"rsrpOffsetSSB"
		},
	{ ATF_POINTER, 5, offsetof(struct Q_OffsetRangeList, rsrqOffsetSSB),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Q_OffsetRange,
		0,
		{ 0, 0, 0 },
		&asn_DFL_3_cmp_15,	/* Compare DEFAULT 15 */
		&asn_DFL_3_set_15,	/* Set DEFAULT 15 */
		"rsrqOffsetSSB"
		},
	{ ATF_POINTER, 4, offsetof(struct Q_OffsetRangeList, sinrOffsetSSB),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Q_OffsetRange,
		0,
		{ 0, 0, 0 },
		&asn_DFL_4_cmp_15,	/* Compare DEFAULT 15 */
		&asn_DFL_4_set_15,	/* Set DEFAULT 15 */
		"sinrOffsetSSB"
		},
	{ ATF_POINTER, 3, offsetof(struct Q_OffsetRangeList, rsrpOffsetCSI_RS),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Q_OffsetRange,
		0,
		{ 0, 0, 0 },
		&asn_DFL_5_cmp_15,	/* Compare DEFAULT 15 */
		&asn_DFL_5_set_15,	/* Set DEFAULT 15 */
		"rsrpOffsetCSI-RS"
		},
	{ ATF_POINTER, 2, offsetof(struct Q_OffsetRangeList, rsrqOffsetCSI_RS),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Q_OffsetRange,
		0,
		{ 0, 0, 0 },
		&asn_DFL_6_cmp_15,	/* Compare DEFAULT 15 */
		&asn_DFL_6_set_15,	/* Set DEFAULT 15 */
		"rsrqOffsetCSI-RS"
		},
	{ ATF_POINTER, 1, offsetof(struct Q_OffsetRangeList, sinrOffsetCSI_RS),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Q_OffsetRange,
		0,
		{ 0, 0, 0 },
		&asn_DFL_7_cmp_15,	/* Compare DEFAULT 15 */
		&asn_DFL_7_set_15,	/* Set DEFAULT 15 */
		"sinrOffsetCSI-RS"
		},
};
static const int asn_MAP_Q_OffsetRangeList_oms_1[] = { 0, 1, 2, 3, 4, 5 };
static const ber_tlv_tag_t asn_DEF_Q_OffsetRangeList_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Q_OffsetRangeList_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* rsrpOffsetSSB */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* rsrqOffsetSSB */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* sinrOffsetSSB */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* rsrpOffsetCSI-RS */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* rsrqOffsetCSI-RS */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* sinrOffsetCSI-RS */
};
asn_SEQUENCE_specifics_t asn_SPC_Q_OffsetRangeList_specs_1 = {
	sizeof(struct Q_OffsetRangeList),
	offsetof(struct Q_OffsetRangeList, _asn_ctx),
	asn_MAP_Q_OffsetRangeList_tag2el_1,
	6,	/* Count of tags in the map */
	asn_MAP_Q_OffsetRangeList_oms_1,	/* Optional members */
	6, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Q_OffsetRangeList = {
	"Q-OffsetRangeList",
	"Q-OffsetRangeList",
	&asn_OP_SEQUENCE,
	asn_DEF_Q_OffsetRangeList_tags_1,
	sizeof(asn_DEF_Q_OffsetRangeList_tags_1)
		/sizeof(asn_DEF_Q_OffsetRangeList_tags_1[0]), /* 1 */
	asn_DEF_Q_OffsetRangeList_tags_1,	/* Same as above */
	sizeof(asn_DEF_Q_OffsetRangeList_tags_1)
		/sizeof(asn_DEF_Q_OffsetRangeList_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_Q_OffsetRangeList_1,
	6,	/* Elements count */
	&asn_SPC_Q_OffsetRangeList_specs_1	/* Additional specs */
};

