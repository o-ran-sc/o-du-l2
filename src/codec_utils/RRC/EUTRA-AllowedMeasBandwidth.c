/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#include "EUTRA-AllowedMeasBandwidth.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
asn_per_constraints_t asn_PER_type_EUTRA_AllowedMeasBandwidth_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  5 }	/* (0..5) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
static const asn_INTEGER_enum_map_t asn_MAP_EUTRA_AllowedMeasBandwidth_value2enum_1[] = {
	{ 0,	4,	"mbw6" },
	{ 1,	5,	"mbw15" },
	{ 2,	5,	"mbw25" },
	{ 3,	5,	"mbw50" },
	{ 4,	5,	"mbw75" },
	{ 5,	6,	"mbw100" }
};
static const unsigned int asn_MAP_EUTRA_AllowedMeasBandwidth_enum2value_1[] = {
	5,	/* mbw100(5) */
	1,	/* mbw15(1) */
	2,	/* mbw25(2) */
	3,	/* mbw50(3) */
	0,	/* mbw6(0) */
	4	/* mbw75(4) */
};
const asn_INTEGER_specifics_t asn_SPC_EUTRA_AllowedMeasBandwidth_specs_1 = {
	asn_MAP_EUTRA_AllowedMeasBandwidth_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_EUTRA_AllowedMeasBandwidth_enum2value_1,	/* N => "tag"; sorted by N */
	6,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_EUTRA_AllowedMeasBandwidth_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_EUTRA_AllowedMeasBandwidth = {
	"EUTRA-AllowedMeasBandwidth",
	"EUTRA-AllowedMeasBandwidth",
	&asn_OP_NativeEnumerated,
	asn_DEF_EUTRA_AllowedMeasBandwidth_tags_1,
	sizeof(asn_DEF_EUTRA_AllowedMeasBandwidth_tags_1)
		/sizeof(asn_DEF_EUTRA_AllowedMeasBandwidth_tags_1[0]), /* 1 */
	asn_DEF_EUTRA_AllowedMeasBandwidth_tags_1,	/* Same as above */
	sizeof(asn_DEF_EUTRA_AllowedMeasBandwidth_tags_1)
		/sizeof(asn_DEF_EUTRA_AllowedMeasBandwidth_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_EUTRA_AllowedMeasBandwidth_constr_1,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_EUTRA_AllowedMeasBandwidth_specs_1	/* Additional specs */
};

