/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "P0-PUSCH-AlphaSet.h"

static int
memb_p0_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -16 && value <= 15)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_p0_constr_3 CC_NOTUSED = {
	{ 1, 0 }	/* (-16..15) */,
	-1};
static asn_per_constraints_t asn_PER_memb_p0_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 5,  5, -16,  15 }	/* (-16..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_P0_PUSCH_AlphaSet_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct P0_PUSCH_AlphaSet, p0_PUSCH_AlphaSetId),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_P0_PUSCH_AlphaSetId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"p0-PUSCH-AlphaSetId"
		},
	{ ATF_POINTER, 2, offsetof(struct P0_PUSCH_AlphaSet, p0),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_p0_constr_3, &asn_PER_memb_p0_constr_3,  memb_p0_constraint_1 },
		0, 0, /* No default value */
		"p0"
		},
	{ ATF_POINTER, 1, offsetof(struct P0_PUSCH_AlphaSet, alpha),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Alpha,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"alpha"
		},
};
static const int asn_MAP_P0_PUSCH_AlphaSet_oms_1[] = { 1, 2 };
static const ber_tlv_tag_t asn_DEF_P0_PUSCH_AlphaSet_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_P0_PUSCH_AlphaSet_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* p0-PUSCH-AlphaSetId */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* p0 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* alpha */
};
asn_SEQUENCE_specifics_t asn_SPC_P0_PUSCH_AlphaSet_specs_1 = {
	sizeof(struct P0_PUSCH_AlphaSet),
	offsetof(struct P0_PUSCH_AlphaSet, _asn_ctx),
	asn_MAP_P0_PUSCH_AlphaSet_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_P0_PUSCH_AlphaSet_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_P0_PUSCH_AlphaSet = {
	"P0-PUSCH-AlphaSet",
	"P0-PUSCH-AlphaSet",
	&asn_OP_SEQUENCE,
	asn_DEF_P0_PUSCH_AlphaSet_tags_1,
	sizeof(asn_DEF_P0_PUSCH_AlphaSet_tags_1)
		/sizeof(asn_DEF_P0_PUSCH_AlphaSet_tags_1[0]), /* 1 */
	asn_DEF_P0_PUSCH_AlphaSet_tags_1,	/* Same as above */
	sizeof(asn_DEF_P0_PUSCH_AlphaSet_tags_1)
		/sizeof(asn_DEF_P0_PUSCH_AlphaSet_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_P0_PUSCH_AlphaSet_1,
	3,	/* Elements count */
	&asn_SPC_P0_PUSCH_AlphaSet_specs_1	/* Additional specs */
};

