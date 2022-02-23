/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SystemInformation-IEs.h"

#include "SIB2.h"
#include "SIB3.h"
#include "SIB4.h"
#include "SIB5.h"
#include "SIB6.h"
#include "SIB7.h"
#include "SIB8.h"
#include "SIB9.h"
static int
memb_sib_TypeAndInfo_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 32)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_Member_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_Member_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  3,  3,  0,  7 }	/* (0..7,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_sib_TypeAndInfo_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_type_sib_TypeAndInfo_constr_2 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_sib_TypeAndInfo_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_memb_sib_TypeAndInfo_constr_2 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_Member_3[] = {
	{ ATF_POINTER, 0, offsetof(struct SystemInformation_IEs__sib_TypeAndInfo__Member, choice.sib2),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SIB2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sib2"
		},
	{ ATF_POINTER, 0, offsetof(struct SystemInformation_IEs__sib_TypeAndInfo__Member, choice.sib3),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SIB3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sib3"
		},
	{ ATF_POINTER, 0, offsetof(struct SystemInformation_IEs__sib_TypeAndInfo__Member, choice.sib4),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SIB4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sib4"
		},
	{ ATF_POINTER, 0, offsetof(struct SystemInformation_IEs__sib_TypeAndInfo__Member, choice.sib5),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SIB5,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sib5"
		},
	{ ATF_POINTER, 0, offsetof(struct SystemInformation_IEs__sib_TypeAndInfo__Member, choice.sib6),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SIB6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sib6"
		},
	{ ATF_POINTER, 0, offsetof(struct SystemInformation_IEs__sib_TypeAndInfo__Member, choice.sib7),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SIB7,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sib7"
		},
	{ ATF_POINTER, 0, offsetof(struct SystemInformation_IEs__sib_TypeAndInfo__Member, choice.sib8),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SIB8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sib8"
		},
	{ ATF_POINTER, 0, offsetof(struct SystemInformation_IEs__sib_TypeAndInfo__Member, choice.sib9),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SIB9,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sib9"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_Member_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* sib2 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* sib3 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* sib4 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* sib5 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* sib6 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* sib7 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* sib8 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 } /* sib9 */
};
static asn_CHOICE_specifics_t asn_SPC_Member_specs_3 = {
	sizeof(struct SystemInformation_IEs__sib_TypeAndInfo__Member),
	offsetof(struct SystemInformation_IEs__sib_TypeAndInfo__Member, _asn_ctx),
	offsetof(struct SystemInformation_IEs__sib_TypeAndInfo__Member, present),
	sizeof(((struct SystemInformation_IEs__sib_TypeAndInfo__Member *)0)->present),
	asn_MAP_Member_tag2el_3,
	8,	/* Count of tags in the map */
	0, 0,
	8	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_Member_3 = {
	"CHOICE",
	"CHOICE",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_Member_constr_3, &asn_PER_type_Member_constr_3, CHOICE_constraint },
	asn_MBR_Member_3,
	8,	/* Elements count */
	&asn_SPC_Member_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_sib_TypeAndInfo_2[] = {
	{ ATF_POINTER, 0, 0,
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_Member_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_sib_TypeAndInfo_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_sib_TypeAndInfo_specs_2 = {
	sizeof(struct SystemInformation_IEs__sib_TypeAndInfo),
	offsetof(struct SystemInformation_IEs__sib_TypeAndInfo, _asn_ctx),
	2,	/* XER encoding is XMLValueList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_sib_TypeAndInfo_2 = {
	"sib-TypeAndInfo",
	"sib-TypeAndInfo",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_sib_TypeAndInfo_tags_2,
	sizeof(asn_DEF_sib_TypeAndInfo_tags_2)
		/sizeof(asn_DEF_sib_TypeAndInfo_tags_2[0]) - 1, /* 1 */
	asn_DEF_sib_TypeAndInfo_tags_2,	/* Same as above */
	sizeof(asn_DEF_sib_TypeAndInfo_tags_2)
		/sizeof(asn_DEF_sib_TypeAndInfo_tags_2[0]), /* 2 */
	{ &asn_OER_type_sib_TypeAndInfo_constr_2, &asn_PER_type_sib_TypeAndInfo_constr_2, SEQUENCE_OF_constraint },
	asn_MBR_sib_TypeAndInfo_2,
	1,	/* Single element */
	&asn_SPC_sib_TypeAndInfo_specs_2	/* Additional specs */
};

static const ber_tlv_tag_t asn_DEF_nonCriticalExtension_tags_14[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SEQUENCE_specifics_t asn_SPC_nonCriticalExtension_specs_14 = {
	sizeof(struct SystemInformation_IEs__nonCriticalExtension),
	offsetof(struct SystemInformation_IEs__nonCriticalExtension, _asn_ctx),
	0,	/* No top level tags */
	0,	/* No tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_nonCriticalExtension_14 = {
	"nonCriticalExtension",
	"nonCriticalExtension",
	&asn_OP_SEQUENCE,
	asn_DEF_nonCriticalExtension_tags_14,
	sizeof(asn_DEF_nonCriticalExtension_tags_14)
		/sizeof(asn_DEF_nonCriticalExtension_tags_14[0]) - 1, /* 1 */
	asn_DEF_nonCriticalExtension_tags_14,	/* Same as above */
	sizeof(asn_DEF_nonCriticalExtension_tags_14)
		/sizeof(asn_DEF_nonCriticalExtension_tags_14[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	0, 0,	/* No members */
	&asn_SPC_nonCriticalExtension_specs_14	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SystemInformation_IEs_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SystemInformation_IEs, sib_TypeAndInfo),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_sib_TypeAndInfo_2,
		0,
		{ &asn_OER_memb_sib_TypeAndInfo_constr_2, &asn_PER_memb_sib_TypeAndInfo_constr_2,  memb_sib_TypeAndInfo_constraint_1 },
		0, 0, /* No default value */
		"sib-TypeAndInfo"
		},
	{ ATF_POINTER, 2, offsetof(struct SystemInformation_IEs, lateNonCriticalExtension),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OCTET_STRING,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"lateNonCriticalExtension"
		},
	{ ATF_POINTER, 1, offsetof(struct SystemInformation_IEs, nonCriticalExtension),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_nonCriticalExtension_14,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"nonCriticalExtension"
		},
};
static const int asn_MAP_SystemInformation_IEs_oms_1[] = { 1, 2 };
static const ber_tlv_tag_t asn_DEF_SystemInformation_IEs_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SystemInformation_IEs_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* sib-TypeAndInfo */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* lateNonCriticalExtension */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* nonCriticalExtension */
};
asn_SEQUENCE_specifics_t asn_SPC_SystemInformation_IEs_specs_1 = {
	sizeof(struct SystemInformation_IEs),
	offsetof(struct SystemInformation_IEs, _asn_ctx),
	asn_MAP_SystemInformation_IEs_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_SystemInformation_IEs_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SystemInformation_IEs = {
	"SystemInformation-IEs",
	"SystemInformation-IEs",
	&asn_OP_SEQUENCE,
	asn_DEF_SystemInformation_IEs_tags_1,
	sizeof(asn_DEF_SystemInformation_IEs_tags_1)
		/sizeof(asn_DEF_SystemInformation_IEs_tags_1[0]), /* 1 */
	asn_DEF_SystemInformation_IEs_tags_1,	/* Same as above */
	sizeof(asn_DEF_SystemInformation_IEs_tags_1)
		/sizeof(asn_DEF_SystemInformation_IEs_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SystemInformation_IEs_1,
	3,	/* Elements count */
	&asn_SPC_SystemInformation_IEs_specs_1	/* Additional specs */
};

