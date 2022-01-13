/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "2022_E2AP.asn1"
 * 	`asn1c -D ./E2AP/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "E2nodeComponentConfigUpdate-List.h"

#include "ProtocolIE-SingleContainerE2.h"
static asn_oer_constraints_t asn_OER_type_E2nodeComponentConfigUpdate_List_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..1024)) */};
static asn_per_constraints_t asn_PER_type_E2nodeComponentConfigUpdate_List_constr_1 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 10,  10,  1,  1024 }	/* (SIZE(1..1024)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_E2nodeComponentConfigUpdate_List_1[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ProtocolIE_SingleContainerE2_1928P8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_E2nodeComponentConfigUpdate_List_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_E2nodeComponentConfigUpdate_List_specs_1 = {
	sizeof(struct E2nodeComponentConfigUpdate_List),
	offsetof(struct E2nodeComponentConfigUpdate_List, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
asn_TYPE_descriptor_t asn_DEF_E2nodeComponentConfigUpdate_List = {
	"E2nodeComponentConfigUpdate-List",
	"E2nodeComponentConfigUpdate-List",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_E2nodeComponentConfigUpdate_List_tags_1,
	sizeof(asn_DEF_E2nodeComponentConfigUpdate_List_tags_1)
		/sizeof(asn_DEF_E2nodeComponentConfigUpdate_List_tags_1[0]), /* 1 */
	asn_DEF_E2nodeComponentConfigUpdate_List_tags_1,	/* Same as above */
	sizeof(asn_DEF_E2nodeComponentConfigUpdate_List_tags_1)
		/sizeof(asn_DEF_E2nodeComponentConfigUpdate_List_tags_1[0]), /* 1 */
	{ &asn_OER_type_E2nodeComponentConfigUpdate_List_constr_1, &asn_PER_type_E2nodeComponentConfigUpdate_List_constr_1, SEQUENCE_OF_constraint },
	asn_MBR_E2nodeComponentConfigUpdate_List_1,
	1,	/* Single element */
	&asn_SPC_E2nodeComponentConfigUpdate_List_specs_1	/* Additional specs */
};

