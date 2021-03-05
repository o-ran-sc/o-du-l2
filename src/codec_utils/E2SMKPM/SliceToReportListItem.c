/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "codeathon/E2SM_KPM.asn1"
 * 	`asn1c -D ./codeathon/e2smkpm/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SliceToReportListItem.h"

#include "FQIPERSlicesPerPlmnListItem.h"
static int
memb_fQIPERSlicesPerPlmnList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	/* Determine the number of elements */
	size = _A_CSEQUENCE_FROM_VOID(sptr)->count;
	
	if((size >= 1 && size <= 64)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_fQIPERSlicesPerPlmnList_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..64)) */};
static asn_per_constraints_t asn_PER_type_fQIPERSlicesPerPlmnList_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 6,  6,  1,  64 }	/* (SIZE(1..64)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_fQIPERSlicesPerPlmnList_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..64)) */};
static asn_per_constraints_t asn_PER_memb_fQIPERSlicesPerPlmnList_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 6,  6,  1,  64 }	/* (SIZE(1..64)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_fQIPERSlicesPerPlmnList_3[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_FQIPERSlicesPerPlmnListItem,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_fQIPERSlicesPerPlmnList_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_fQIPERSlicesPerPlmnList_specs_3 = {
	sizeof(struct SliceToReportListItem__fQIPERSlicesPerPlmnList),
	offsetof(struct SliceToReportListItem__fQIPERSlicesPerPlmnList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_fQIPERSlicesPerPlmnList_3 = {
	"fQIPERSlicesPerPlmnList",
	"fQIPERSlicesPerPlmnList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_fQIPERSlicesPerPlmnList_tags_3,
	sizeof(asn_DEF_fQIPERSlicesPerPlmnList_tags_3)
		/sizeof(asn_DEF_fQIPERSlicesPerPlmnList_tags_3[0]) - 1, /* 1 */
	asn_DEF_fQIPERSlicesPerPlmnList_tags_3,	/* Same as above */
	sizeof(asn_DEF_fQIPERSlicesPerPlmnList_tags_3)
		/sizeof(asn_DEF_fQIPERSlicesPerPlmnList_tags_3[0]), /* 2 */
	{ &asn_OER_type_fQIPERSlicesPerPlmnList_constr_3, &asn_PER_type_fQIPERSlicesPerPlmnList_constr_3, SEQUENCE_OF_constraint },
	asn_MBR_fQIPERSlicesPerPlmnList_3,
	1,	/* Single element */
	&asn_SPC_fQIPERSlicesPerPlmnList_specs_3	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SliceToReportListItem_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SliceToReportListItem, sliceID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SNSSAI,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sliceID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SliceToReportListItem, fQIPERSlicesPerPlmnList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_fQIPERSlicesPerPlmnList_3,
		0,
		{ &asn_OER_memb_fQIPERSlicesPerPlmnList_constr_3, &asn_PER_memb_fQIPERSlicesPerPlmnList_constr_3,  memb_fQIPERSlicesPerPlmnList_constraint_1 },
		0, 0, /* No default value */
		"fQIPERSlicesPerPlmnList"
		},
};
static const ber_tlv_tag_t asn_DEF_SliceToReportListItem_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SliceToReportListItem_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* sliceID */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* fQIPERSlicesPerPlmnList */
};
asn_SEQUENCE_specifics_t asn_SPC_SliceToReportListItem_specs_1 = {
	sizeof(struct SliceToReportListItem),
	offsetof(struct SliceToReportListItem, _asn_ctx),
	asn_MAP_SliceToReportListItem_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SliceToReportListItem = {
	"SliceToReportListItem",
	"SliceToReportListItem",
	&asn_OP_SEQUENCE,
	asn_DEF_SliceToReportListItem_tags_1,
	sizeof(asn_DEF_SliceToReportListItem_tags_1)
		/sizeof(asn_DEF_SliceToReportListItem_tags_1[0]), /* 1 */
	asn_DEF_SliceToReportListItem_tags_1,	/* Same as above */
	sizeof(asn_DEF_SliceToReportListItem_tags_1)
		/sizeof(asn_DEF_SliceToReportListItem_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SliceToReportListItem_1,
	2,	/* Elements count */
	&asn_SPC_SliceToReportListItem_specs_1	/* Additional specs */
};

