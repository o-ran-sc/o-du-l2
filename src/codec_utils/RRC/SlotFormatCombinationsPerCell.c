/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "SlotFormatCombinationsPerCell.h"

#include "SlotFormatCombination.h"
static int
memb_slotFormatCombinations_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 512)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_positionInDCI_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 127)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_slotFormatCombinations_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..512)) */};
static asn_per_constraints_t asn_PER_type_slotFormatCombinations_constr_5 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 9,  9,  1,  512 }	/* (SIZE(1..512)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_slotFormatCombinations_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..512)) */};
static asn_per_constraints_t asn_PER_memb_slotFormatCombinations_constr_5 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 9,  9,  1,  512 }	/* (SIZE(1..512)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_positionInDCI_constr_7 CC_NOTUSED = {
	{ 1, 1 }	/* (0..127) */,
	-1};
static asn_per_constraints_t asn_PER_memb_positionInDCI_constr_7 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7,  0,  127 }	/* (0..127) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_slotFormatCombinations_5[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_SlotFormatCombination,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_slotFormatCombinations_tags_5[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_slotFormatCombinations_specs_5 = {
	sizeof(struct SlotFormatCombinationsPerCell__slotFormatCombinations),
	offsetof(struct SlotFormatCombinationsPerCell__slotFormatCombinations, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_slotFormatCombinations_5 = {
	"slotFormatCombinations",
	"slotFormatCombinations",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_slotFormatCombinations_tags_5,
	sizeof(asn_DEF_slotFormatCombinations_tags_5)
		/sizeof(asn_DEF_slotFormatCombinations_tags_5[0]) - 1, /* 1 */
	asn_DEF_slotFormatCombinations_tags_5,	/* Same as above */
	sizeof(asn_DEF_slotFormatCombinations_tags_5)
		/sizeof(asn_DEF_slotFormatCombinations_tags_5[0]), /* 2 */
	{ &asn_OER_type_slotFormatCombinations_constr_5, &asn_PER_type_slotFormatCombinations_constr_5, SEQUENCE_OF_constraint },
	asn_MBR_slotFormatCombinations_5,
	1,	/* Single element */
	&asn_SPC_slotFormatCombinations_specs_5	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SlotFormatCombinationsPerCell_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SlotFormatCombinationsPerCell, servingCellId),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ServCellIndexRrc,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"servingCellId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SlotFormatCombinationsPerCell, subcarrierSpacing),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SubcarrierSpacing,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"subcarrierSpacing"
		},
	{ ATF_POINTER, 3, offsetof(struct SlotFormatCombinationsPerCell, subcarrierSpacing2),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SubcarrierSpacing,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"subcarrierSpacing2"
		},
	{ ATF_POINTER, 2, offsetof(struct SlotFormatCombinationsPerCell, slotFormatCombinations),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		0,
		&asn_DEF_slotFormatCombinations_5,
		0,
		{ &asn_OER_memb_slotFormatCombinations_constr_5, &asn_PER_memb_slotFormatCombinations_constr_5,  memb_slotFormatCombinations_constraint_1 },
		0, 0, /* No default value */
		"slotFormatCombinations"
		},
	{ ATF_POINTER, 1, offsetof(struct SlotFormatCombinationsPerCell, positionInDCI),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_positionInDCI_constr_7, &asn_PER_memb_positionInDCI_constr_7,  memb_positionInDCI_constraint_1 },
		0, 0, /* No default value */
		"positionInDCI"
		},
};
static const int asn_MAP_SlotFormatCombinationsPerCell_oms_1[] = { 2, 3, 4 };
static const ber_tlv_tag_t asn_DEF_SlotFormatCombinationsPerCell_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SlotFormatCombinationsPerCell_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* servingCellId */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* subcarrierSpacing */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* subcarrierSpacing2 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* slotFormatCombinations */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* positionInDCI */
};
asn_SEQUENCE_specifics_t asn_SPC_SlotFormatCombinationsPerCell_specs_1 = {
	sizeof(struct SlotFormatCombinationsPerCell),
	offsetof(struct SlotFormatCombinationsPerCell, _asn_ctx),
	asn_MAP_SlotFormatCombinationsPerCell_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_SlotFormatCombinationsPerCell_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	5,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SlotFormatCombinationsPerCell = {
	"SlotFormatCombinationsPerCell",
	"SlotFormatCombinationsPerCell",
	&asn_OP_SEQUENCE,
	asn_DEF_SlotFormatCombinationsPerCell_tags_1,
	sizeof(asn_DEF_SlotFormatCombinationsPerCell_tags_1)
		/sizeof(asn_DEF_SlotFormatCombinationsPerCell_tags_1[0]), /* 1 */
	asn_DEF_SlotFormatCombinationsPerCell_tags_1,	/* Same as above */
	sizeof(asn_DEF_SlotFormatCombinationsPerCell_tags_1)
		/sizeof(asn_DEF_SlotFormatCombinationsPerCell_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SlotFormatCombinationsPerCell_1,
	5,	/* Elements count */
	&asn_SPC_SlotFormatCombinationsPerCell_specs_1	/* Additional specs */
};
