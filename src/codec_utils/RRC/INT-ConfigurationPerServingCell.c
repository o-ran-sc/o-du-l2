/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "INT-ConfigurationPerServingCell.h"

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
	
	if((value >= 0 && value <= 125)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_positionInDCI_constr_3 CC_NOTUSED = {
	{ 1, 1 }	/* (0..125) */,
	-1};
static asn_per_constraints_t asn_PER_memb_positionInDCI_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7,  0,  125 }	/* (0..125) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_INT_ConfigurationPerServingCell_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct INT_ConfigurationPerServingCell, servingCellId),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ServCellIndex,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"servingCellId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct INT_ConfigurationPerServingCell, positionInDCI),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_positionInDCI_constr_3, &asn_PER_memb_positionInDCI_constr_3,  memb_positionInDCI_constraint_1 },
		0, 0, /* No default value */
		"positionInDCI"
		},
};
static const ber_tlv_tag_t asn_DEF_INT_ConfigurationPerServingCell_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_INT_ConfigurationPerServingCell_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* servingCellId */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* positionInDCI */
};
asn_SEQUENCE_specifics_t asn_SPC_INT_ConfigurationPerServingCell_specs_1 = {
	sizeof(struct INT_ConfigurationPerServingCell),
	offsetof(struct INT_ConfigurationPerServingCell, _asn_ctx),
	asn_MAP_INT_ConfigurationPerServingCell_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_INT_ConfigurationPerServingCell = {
	"INT-ConfigurationPerServingCell",
	"INT-ConfigurationPerServingCell",
	&asn_OP_SEQUENCE,
	asn_DEF_INT_ConfigurationPerServingCell_tags_1,
	sizeof(asn_DEF_INT_ConfigurationPerServingCell_tags_1)
		/sizeof(asn_DEF_INT_ConfigurationPerServingCell_tags_1[0]), /* 1 */
	asn_DEF_INT_ConfigurationPerServingCell_tags_1,	/* Same as above */
	sizeof(asn_DEF_INT_ConfigurationPerServingCell_tags_1)
		/sizeof(asn_DEF_INT_ConfigurationPerServingCell_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_INT_ConfigurationPerServingCell_1,
	2,	/* Elements count */
	&asn_SPC_INT_ConfigurationPerServingCell_specs_1	/* Additional specs */
};

