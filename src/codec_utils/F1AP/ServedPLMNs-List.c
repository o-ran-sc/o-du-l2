/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "ServedPLMNs-List.h"

#include "ServedPLMNs-Item.h"
static asn_oer_constraints_t asn_OER_type_ServedPLMNs_List_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..6)) */};
asn_per_constraints_t asn_PER_type_ServedPLMNs_List_constr_1 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 3,  3,  1,  6 }	/* (SIZE(1..6)) */,
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_ServedPLMNs_List_1[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ServedPLMNs_Item,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_ServedPLMNs_List_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_SET_OF_specifics_t asn_SPC_ServedPLMNs_List_specs_1 = {
	sizeof(struct ServedPLMNs_List),
	offsetof(struct ServedPLMNs_List, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
asn_TYPE_descriptor_t asn_DEF_ServedPLMNs_List = {
	"ServedPLMNs-List",
	"ServedPLMNs-List",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_ServedPLMNs_List_tags_1,
	sizeof(asn_DEF_ServedPLMNs_List_tags_1)
		/sizeof(asn_DEF_ServedPLMNs_List_tags_1[0]), /* 1 */
	asn_DEF_ServedPLMNs_List_tags_1,	/* Same as above */
	sizeof(asn_DEF_ServedPLMNs_List_tags_1)
		/sizeof(asn_DEF_ServedPLMNs_List_tags_1[0]), /* 1 */
	{ &asn_OER_type_ServedPLMNs_List_constr_1, &asn_PER_type_ServedPLMNs_List_constr_1, SEQUENCE_OF_constraint },
	asn_MBR_ServedPLMNs_List_1,
	1,	/* Single element */
	&asn_SPC_ServedPLMNs_List_specs_1	/* Additional specs */
};

