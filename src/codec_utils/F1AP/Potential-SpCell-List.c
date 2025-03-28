/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Contents"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#include "Potential-SpCell-List.h"

#include "ProtocolIE-SingleContainer.h"
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
asn_per_constraints_t asn_PER_type_Potential_SpCell_List_constr_1 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 7,  7,  0,  64 }	/* (SIZE(0..64)) */,
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
asn_TYPE_member_t asn_MBR_Potential_SpCell_List_1[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ProtocolIE_SingleContainer_4590P29,
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
		""
		},
};
static const ber_tlv_tag_t asn_DEF_Potential_SpCell_List_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_SET_OF_specifics_t asn_SPC_Potential_SpCell_List_specs_1 = {
	sizeof(struct Potential_SpCell_List),
	offsetof(struct Potential_SpCell_List, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
asn_TYPE_descriptor_t asn_DEF_Potential_SpCell_List = {
	"Potential-SpCell-List",
	"Potential-SpCell-List",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_Potential_SpCell_List_tags_1,
	sizeof(asn_DEF_Potential_SpCell_List_tags_1)
		/sizeof(asn_DEF_Potential_SpCell_List_tags_1[0]), /* 1 */
	asn_DEF_Potential_SpCell_List_tags_1,	/* Same as above */
	sizeof(asn_DEF_Potential_SpCell_List_tags_1)
		/sizeof(asn_DEF_Potential_SpCell_List_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_Potential_SpCell_List_constr_1,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		SEQUENCE_OF_constraint
	},
	asn_MBR_Potential_SpCell_List_1,
	1,	/* Single element */
	&asn_SPC_Potential_SpCell_List_specs_1	/* Additional specs */
};

