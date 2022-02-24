/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "PagingUE-Identity.h"

static asn_oer_constraints_t asn_OER_type_PagingUE_Identity_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_PagingUE_Identity_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  1,  1,  0,  1 }	/* (0..1,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_PagingUE_Identity_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PagingUE_Identity, choice.ng_5G_S_TMSI),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NG_5G_S_TMSI,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ng-5G-S-TMSI"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PagingUE_Identity, choice.fullI_RNTI),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_I_RNTI_Value,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"fullI-RNTI"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_PagingUE_Identity_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ng-5G-S-TMSI */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* fullI-RNTI */
};
asn_CHOICE_specifics_t asn_SPC_PagingUE_Identity_specs_1 = {
	sizeof(struct PagingUE_Identity),
	offsetof(struct PagingUE_Identity, _asn_ctx),
	offsetof(struct PagingUE_Identity, present),
	sizeof(((struct PagingUE_Identity *)0)->present),
	asn_MAP_PagingUE_Identity_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0,
	2	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_PagingUE_Identity = {
	"PagingUE-Identity",
	"PagingUE-Identity",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_PagingUE_Identity_constr_1, &asn_PER_type_PagingUE_Identity_constr_1, CHOICE_constraint },
	asn_MBR_PagingUE_Identity_1,
	2,	/* Elements count */
	&asn_SPC_PagingUE_Identity_specs_1	/* Additional specs */
};

