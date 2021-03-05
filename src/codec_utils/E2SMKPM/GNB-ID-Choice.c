/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "codeathon/E2SM_KPM.asn1"
 * 	`asn1c -D ./codeathon/e2smkpm/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "GNB-ID-Choice.h"

static int
memb_gnb_ID_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size >= 22 && size <= 32)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_gnb_ID_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(22..32)) */};
static asn_per_constraints_t asn_PER_memb_gnb_ID_constr_2 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 4,  4,  22,  32 }	/* (SIZE(22..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_GNB_ID_Choice_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_GNB_ID_Choice_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  0,  0,  0,  0 }	/* (0..0,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_GNB_ID_Choice_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct GNB_ID_Choice, choice.gnb_ID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_gnb_ID_constr_2, &asn_PER_memb_gnb_ID_constr_2,  memb_gnb_ID_constraint_1 },
		0, 0, /* No default value */
		"gnb-ID"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_GNB_ID_Choice_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* gnb-ID */
};
asn_CHOICE_specifics_t asn_SPC_GNB_ID_Choice_specs_1 = {
	sizeof(struct GNB_ID_Choice),
	offsetof(struct GNB_ID_Choice, _asn_ctx),
	offsetof(struct GNB_ID_Choice, present),
	sizeof(((struct GNB_ID_Choice *)0)->present),
	asn_MAP_GNB_ID_Choice_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0,
	1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_GNB_ID_Choice = {
	"GNB-ID-Choice",
	"GNB-ID-Choice",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_GNB_ID_Choice_constr_1, &asn_PER_type_GNB_ID_Choice_constr_1, CHOICE_constraint },
	asn_MBR_GNB_ID_Choice_1,
	1,	/* Elements count */
	&asn_SPC_GNB_ID_Choice_specs_1	/* Additional specs */
};

