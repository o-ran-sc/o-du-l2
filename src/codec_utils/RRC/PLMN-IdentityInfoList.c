/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "PLMN-IdentityInfoList.h"

#include "PLMN-IdentityInfo.h"
static asn_oer_constraints_t asn_OER_type_PLMN_IdentityInfoList_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..12)) */};
asn_per_constraints_t asn_PER_type_PLMN_IdentityInfoList_constr_1 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 4,  4,  1,  12 }	/* (SIZE(1..12)) */,
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_PLMN_IdentityInfoList_1[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_PLMN_IdentityInfo,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_PLMN_IdentityInfoList_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_SET_OF_specifics_t asn_SPC_PLMN_IdentityInfoList_specs_1 = {
	sizeof(struct PLMN_IdentityInfoList),
	offsetof(struct PLMN_IdentityInfoList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
asn_TYPE_descriptor_t asn_DEF_PLMN_IdentityInfoList = {
	"PLMN-IdentityInfoList",
	"PLMN-IdentityInfoList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_PLMN_IdentityInfoList_tags_1,
	sizeof(asn_DEF_PLMN_IdentityInfoList_tags_1)
		/sizeof(asn_DEF_PLMN_IdentityInfoList_tags_1[0]), /* 1 */
	asn_DEF_PLMN_IdentityInfoList_tags_1,	/* Same as above */
	sizeof(asn_DEF_PLMN_IdentityInfoList_tags_1)
		/sizeof(asn_DEF_PLMN_IdentityInfoList_tags_1[0]), /* 1 */
	{ &asn_OER_type_PLMN_IdentityInfoList_constr_1, &asn_PER_type_PLMN_IdentityInfoList_constr_1, SEQUENCE_OF_constraint },
	asn_MBR_PLMN_IdentityInfoList_1,
	1,	/* Single element */
	&asn_SPC_PLMN_IdentityInfoList_specs_1	/* Additional specs */
};

