/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "codeathon/E2AP.asn1"
 * 	`asn1c -D ./newE2AP -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "ENB-ID-Choice.h"

static int
memb_enb_ID_macro_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size == 20)) {
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
memb_enb_ID_shortmacro_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size == 18)) {
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
memb_enb_ID_longmacro_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size == 21)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_enb_ID_macro_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	20	/* (SIZE(20..20)) */};
static asn_per_constraints_t asn_PER_memb_enb_ID_macro_constr_2 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  20,  20 }	/* (SIZE(20..20)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_enb_ID_shortmacro_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	18	/* (SIZE(18..18)) */};
static asn_per_constraints_t asn_PER_memb_enb_ID_shortmacro_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  18,  18 }	/* (SIZE(18..18)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_enb_ID_longmacro_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	21	/* (SIZE(21..21)) */};
static asn_per_constraints_t asn_PER_memb_enb_ID_longmacro_constr_4 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  21,  21 }	/* (SIZE(21..21)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_ENB_ID_Choice_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_ENB_ID_Choice_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  2,  2,  0,  2 }	/* (0..2,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_ENB_ID_Choice_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ENB_ID_Choice, choice.enb_ID_macro),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_enb_ID_macro_constr_2, &asn_PER_memb_enb_ID_macro_constr_2,  memb_enb_ID_macro_constraint_1 },
		0, 0, /* No default value */
		"enb-ID-macro"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ENB_ID_Choice, choice.enb_ID_shortmacro),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_enb_ID_shortmacro_constr_3, &asn_PER_memb_enb_ID_shortmacro_constr_3,  memb_enb_ID_shortmacro_constraint_1 },
		0, 0, /* No default value */
		"enb-ID-shortmacro"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ENB_ID_Choice, choice.enb_ID_longmacro),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_enb_ID_longmacro_constr_4, &asn_PER_memb_enb_ID_longmacro_constr_4,  memb_enb_ID_longmacro_constraint_1 },
		0, 0, /* No default value */
		"enb-ID-longmacro"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_ENB_ID_Choice_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* enb-ID-macro */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* enb-ID-shortmacro */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* enb-ID-longmacro */
};
asn_CHOICE_specifics_t asn_SPC_ENB_ID_Choice_specs_1 = {
	sizeof(struct ENB_ID_Choice),
	offsetof(struct ENB_ID_Choice, _asn_ctx),
	offsetof(struct ENB_ID_Choice, present),
	sizeof(((struct ENB_ID_Choice *)0)->present),
	asn_MAP_ENB_ID_Choice_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0,
	3	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_ENB_ID_Choice = {
	"ENB-ID-Choice",
	"ENB-ID-Choice",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_ENB_ID_Choice_constr_1, &asn_PER_type_ENB_ID_Choice_constr_1, CHOICE_constraint },
	asn_MBR_ENB_ID_Choice_1,
	3,	/* Elements count */
	&asn_SPC_ENB_ID_Choice_specs_1	/* Additional specs */
};

