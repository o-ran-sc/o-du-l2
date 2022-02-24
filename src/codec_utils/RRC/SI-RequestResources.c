/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SI-RequestResources.h"

static int
memb_ra_PreambleStartIndex_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 63)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_ra_AssociationPeriodIndex_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 15)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_ra_ssb_OccasionMaskIndex_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 15)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_ra_PreambleStartIndex_constr_2 CC_NOTUSED = {
	{ 1, 1 }	/* (0..63) */,
	-1};
static asn_per_constraints_t asn_PER_memb_ra_PreambleStartIndex_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 6,  6,  0,  63 }	/* (0..63) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_ra_AssociationPeriodIndex_constr_3 CC_NOTUSED = {
	{ 1, 1 }	/* (0..15) */,
	-1};
static asn_per_constraints_t asn_PER_memb_ra_AssociationPeriodIndex_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  15 }	/* (0..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_ra_ssb_OccasionMaskIndex_constr_4 CC_NOTUSED = {
	{ 1, 1 }	/* (0..15) */,
	-1};
static asn_per_constraints_t asn_PER_memb_ra_ssb_OccasionMaskIndex_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  15 }	/* (0..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_SI_RequestResources_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SI_RequestResources, ra_PreambleStartIndex),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_ra_PreambleStartIndex_constr_2, &asn_PER_memb_ra_PreambleStartIndex_constr_2,  memb_ra_PreambleStartIndex_constraint_1 },
		0, 0, /* No default value */
		"ra-PreambleStartIndex"
		},
	{ ATF_POINTER, 2, offsetof(struct SI_RequestResources, ra_AssociationPeriodIndex),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_ra_AssociationPeriodIndex_constr_3, &asn_PER_memb_ra_AssociationPeriodIndex_constr_3,  memb_ra_AssociationPeriodIndex_constraint_1 },
		0, 0, /* No default value */
		"ra-AssociationPeriodIndex"
		},
	{ ATF_POINTER, 1, offsetof(struct SI_RequestResources, ra_ssb_OccasionMaskIndex),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_ra_ssb_OccasionMaskIndex_constr_4, &asn_PER_memb_ra_ssb_OccasionMaskIndex_constr_4,  memb_ra_ssb_OccasionMaskIndex_constraint_1 },
		0, 0, /* No default value */
		"ra-ssb-OccasionMaskIndex"
		},
};
static const int asn_MAP_SI_RequestResources_oms_1[] = { 1, 2 };
static const ber_tlv_tag_t asn_DEF_SI_RequestResources_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SI_RequestResources_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ra-PreambleStartIndex */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* ra-AssociationPeriodIndex */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* ra-ssb-OccasionMaskIndex */
};
asn_SEQUENCE_specifics_t asn_SPC_SI_RequestResources_specs_1 = {
	sizeof(struct SI_RequestResources),
	offsetof(struct SI_RequestResources, _asn_ctx),
	asn_MAP_SI_RequestResources_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_SI_RequestResources_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SI_RequestResources = {
	"SI-RequestResources",
	"SI-RequestResources",
	&asn_OP_SEQUENCE,
	asn_DEF_SI_RequestResources_tags_1,
	sizeof(asn_DEF_SI_RequestResources_tags_1)
		/sizeof(asn_DEF_SI_RequestResources_tags_1[0]), /* 1 */
	asn_DEF_SI_RequestResources_tags_1,	/* Same as above */
	sizeof(asn_DEF_SI_RequestResources_tags_1)
		/sizeof(asn_DEF_SI_RequestResources_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SI_RequestResources_1,
	3,	/* Elements count */
	&asn_SPC_SI_RequestResources_specs_1	/* Additional specs */
};

