/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "PUSCH-PathlossReferenceRS.h"

static asn_oer_constraints_t asn_OER_type_referenceSignal_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_referenceSignal_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_referenceSignal_3[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PUSCH_PathlossReferenceRS__referenceSignal, choice.ssb_Index),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SSB_Index,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ssb-Index"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PUSCH_PathlossReferenceRS__referenceSignal, choice.csi_RS_Index),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NZP_CSI_RS_ResourceId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"csi-RS-Index"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_referenceSignal_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ssb-Index */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* csi-RS-Index */
};
static asn_CHOICE_specifics_t asn_SPC_referenceSignal_specs_3 = {
	sizeof(struct PUSCH_PathlossReferenceRS__referenceSignal),
	offsetof(struct PUSCH_PathlossReferenceRS__referenceSignal, _asn_ctx),
	offsetof(struct PUSCH_PathlossReferenceRS__referenceSignal, present),
	sizeof(((struct PUSCH_PathlossReferenceRS__referenceSignal *)0)->present),
	asn_MAP_referenceSignal_tag2el_3,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_referenceSignal_3 = {
	"referenceSignal",
	"referenceSignal",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_referenceSignal_constr_3, &asn_PER_type_referenceSignal_constr_3, CHOICE_constraint },
	asn_MBR_referenceSignal_3,
	2,	/* Elements count */
	&asn_SPC_referenceSignal_specs_3	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_PUSCH_PathlossReferenceRS_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PUSCH_PathlossReferenceRS, pusch_PathlossReferenceRS_Id),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUSCH_PathlossReferenceRS_Id,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pusch-PathlossReferenceRS-Id"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PUSCH_PathlossReferenceRS, referenceSignal),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_referenceSignal_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"referenceSignal"
		},
};
static const ber_tlv_tag_t asn_DEF_PUSCH_PathlossReferenceRS_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PUSCH_PathlossReferenceRS_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pusch-PathlossReferenceRS-Id */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* referenceSignal */
};
asn_SEQUENCE_specifics_t asn_SPC_PUSCH_PathlossReferenceRS_specs_1 = {
	sizeof(struct PUSCH_PathlossReferenceRS),
	offsetof(struct PUSCH_PathlossReferenceRS, _asn_ctx),
	asn_MAP_PUSCH_PathlossReferenceRS_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_PUSCH_PathlossReferenceRS = {
	"PUSCH-PathlossReferenceRS",
	"PUSCH-PathlossReferenceRS",
	&asn_OP_SEQUENCE,
	asn_DEF_PUSCH_PathlossReferenceRS_tags_1,
	sizeof(asn_DEF_PUSCH_PathlossReferenceRS_tags_1)
		/sizeof(asn_DEF_PUSCH_PathlossReferenceRS_tags_1[0]), /* 1 */
	asn_DEF_PUSCH_PathlossReferenceRS_tags_1,	/* Same as above */
	sizeof(asn_DEF_PUSCH_PathlossReferenceRS_tags_1)
		/sizeof(asn_DEF_PUSCH_PathlossReferenceRS_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_PUSCH_PathlossReferenceRS_1,
	2,	/* Elements count */
	&asn_SPC_PUSCH_PathlossReferenceRS_specs_1	/* Additional specs */
};

