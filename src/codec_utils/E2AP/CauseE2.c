/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "2022_E2AP.asn1"
 * 	`asn1c -D ./E2AP/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "CauseE2.h"

static asn_oer_constraints_t asn_OER_type_CauseE2_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_CauseE2_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  3,  3,  0,  5 }	/* (0..5,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_CauseE2_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CauseE2, choice.ricRequest),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CauseRICrequest,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ricRequest"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CauseE2, choice.ricService),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CauseRICservice,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ricService"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CauseE2, choice.e2Node),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CauseE2node,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"e2Node"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CauseE2, choice.transport),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CauseTransport,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"transport"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CauseE2, choice.protocol),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CauseProtocol,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"protocol"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CauseE2, choice.misc),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CauseMisc,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"misc"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_CauseE2_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ricRequest */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* ricService */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* e2Node */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* transport */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* protocol */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* misc */
};
asn_CHOICE_specifics_t asn_SPC_CauseE2_specs_1 = {
	sizeof(struct CauseE2),
	offsetof(struct CauseE2, _asn_ctx),
	offsetof(struct CauseE2, present),
	sizeof(((struct CauseE2 *)0)->present),
	asn_MAP_CauseE2_tag2el_1,
	6,	/* Count of tags in the map */
	0, 0,
	6	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_CauseE2 = {
	"CauseE2",
	"CauseE2",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_CauseE2_constr_1, &asn_PER_type_CauseE2_constr_1, CHOICE_constraint },
	asn_MBR_CauseE2_1,
	6,	/* Elements count */
	&asn_SPC_CauseE2_specs_1	/* Additional specs */
};

