/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "PLMN-Identity-EUTRA-5GC.h"

#include "PLMN-IdentitY.h"
static int
memb_plmn_index_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 12)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_plmn_index_constr_3 CC_NOTUSED = {
	{ 1, 1 }	/* (1..12) */,
	-1};
static asn_per_constraints_t asn_PER_memb_plmn_index_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  1,  12 }	/* (1..12) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_PLMN_Identity_EUTRA_5GC_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_PLMN_Identity_EUTRA_5GC_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_PLMN_Identity_EUTRA_5GC_1[] = {
	{ ATF_POINTER, 0, offsetof(struct PLMN_Identity_EUTRA_5GC, choice.plmn_Identity_EUTRA_5GC),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PLMN_IdentitY,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"plmn-Identity-EUTRA-5GC"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PLMN_Identity_EUTRA_5GC, choice.plmn_index),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_plmn_index_constr_3, &asn_PER_memb_plmn_index_constr_3,  memb_plmn_index_constraint_1 },
		0, 0, /* No default value */
		"plmn-index"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_PLMN_Identity_EUTRA_5GC_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* plmn-Identity-EUTRA-5GC */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* plmn-index */
};
asn_CHOICE_specifics_t asn_SPC_PLMN_Identity_EUTRA_5GC_specs_1 = {
	sizeof(struct PLMN_Identity_EUTRA_5GC),
	offsetof(struct PLMN_Identity_EUTRA_5GC, _asn_ctx),
	offsetof(struct PLMN_Identity_EUTRA_5GC, present),
	sizeof(((struct PLMN_Identity_EUTRA_5GC *)0)->present),
	asn_MAP_PLMN_Identity_EUTRA_5GC_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_PLMN_Identity_EUTRA_5GC = {
	"PLMN-Identity-EUTRA-5GC",
	"PLMN-Identity-EUTRA-5GC",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_PLMN_Identity_EUTRA_5GC_constr_1, &asn_PER_type_PLMN_Identity_EUTRA_5GC_constr_1, CHOICE_constraint },
	asn_MBR_PLMN_Identity_EUTRA_5GC_1,
	2,	/* Elements count */
	&asn_SPC_PLMN_Identity_EUTRA_5GC_specs_1	/* Additional specs */
};

