/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#include "CellAccessRelatedInfo.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_type_cellReservedForOtherUse_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
static const asn_INTEGER_enum_map_t asn_MAP_cellReservedForOtherUse_value2enum_3[] = {
	{ 0,	4,	"true" }
};
static const unsigned int asn_MAP_cellReservedForOtherUse_enum2value_3[] = {
	0	/* true(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_cellReservedForOtherUse_specs_3 = {
	asn_MAP_cellReservedForOtherUse_value2enum_3,	/* "tag" => N; sorted by tag */
	asn_MAP_cellReservedForOtherUse_enum2value_3,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_cellReservedForOtherUse_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_cellReservedForOtherUse_3 = {
	"cellReservedForOtherUse",
	"cellReservedForOtherUse",
	&asn_OP_NativeEnumerated,
	asn_DEF_cellReservedForOtherUse_tags_3,
	sizeof(asn_DEF_cellReservedForOtherUse_tags_3)
		/sizeof(asn_DEF_cellReservedForOtherUse_tags_3[0]) - 1, /* 1 */
	asn_DEF_cellReservedForOtherUse_tags_3,	/* Same as above */
	sizeof(asn_DEF_cellReservedForOtherUse_tags_3)
		/sizeof(asn_DEF_cellReservedForOtherUse_tags_3[0]), /* 2 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_cellReservedForOtherUse_constr_3,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_cellReservedForOtherUse_specs_3	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_CellAccessRelatedInfo_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CellAccessRelatedInfo, plmn_IdentityList),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PLMN_IdentityInfoList,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"plmn-IdentityList"
		},
	{ ATF_POINTER, 1, offsetof(struct CellAccessRelatedInfo, cellReservedForOtherUse),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_cellReservedForOtherUse_3,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"cellReservedForOtherUse"
		},
};
static const int asn_MAP_CellAccessRelatedInfo_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_CellAccessRelatedInfo_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CellAccessRelatedInfo_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* plmn-IdentityList */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* cellReservedForOtherUse */
};
asn_SEQUENCE_specifics_t asn_SPC_CellAccessRelatedInfo_specs_1 = {
	sizeof(struct CellAccessRelatedInfo),
	offsetof(struct CellAccessRelatedInfo, _asn_ctx),
	asn_MAP_CellAccessRelatedInfo_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_CellAccessRelatedInfo_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_CellAccessRelatedInfo = {
	"CellAccessRelatedInfo",
	"CellAccessRelatedInfo",
	&asn_OP_SEQUENCE,
	asn_DEF_CellAccessRelatedInfo_tags_1,
	sizeof(asn_DEF_CellAccessRelatedInfo_tags_1)
		/sizeof(asn_DEF_CellAccessRelatedInfo_tags_1[0]), /* 1 */
	asn_DEF_CellAccessRelatedInfo_tags_1,	/* Same as above */
	sizeof(asn_DEF_CellAccessRelatedInfo_tags_1)
		/sizeof(asn_DEF_CellAccessRelatedInfo_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		SEQUENCE_constraint
	},
	asn_MBR_CellAccessRelatedInfo_1,
	2,	/* Elements count */
	&asn_SPC_CellAccessRelatedInfo_specs_1	/* Additional specs */
};

