/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#include "CauseRadioNetwork.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
asn_per_constraints_t asn_PER_type_CauseRadioNetwork_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  4,  4,  0,  10 }	/* (0..10,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
static const asn_INTEGER_enum_map_t asn_MAP_CauseRadioNetwork_value2enum_1[] = {
	{ 0,	11,	"unspecified" },
	{ 1,	14,	"rl-failure-rlc" },
	{ 2,	46,	"unknown-or-already-allocated-gnb-cu-ue-f1ap-id" },
	{ 3,	46,	"unknown-or-already-allocated-gnd-du-ue-f1ap-id" },
	{ 4,	42,	"unknown-or-inconsistent-pair-of-ue-f1ap-id" },
	{ 5,	32,	"interaction-with-other-procedure" },
	{ 6,	23,	"not-supported-qci-Value" },
	{ 7,	34,	"action-desirable-for-radio-reasons" },
	{ 8,	28,	"no-radio-resources-available" },
	{ 9,	19,	"procedure-cancelled" },
	{ 10,	14,	"normal-release" },
	{ 11,	18,	"cell-not-available" },
	{ 12,	17,	"rl-failure-others" },
	{ 13,	12,	"ue-rejection" },
	{ 14,	37,	"resources-not-available-for-the-slice" },
	{ 15,	30,	"amf-initiated-abnormal-release" },
	{ 16,	26,	"release-due-to-pre-emption" }
	/* This list is extensible */
};
static const unsigned int asn_MAP_CauseRadioNetwork_enum2value_1[] = {
	7,	/* action-desirable-for-radio-reasons(7) */
	15,	/* amf-initiated-abnormal-release(15) */
	11,	/* cell-not-available(11) */
	5,	/* interaction-with-other-procedure(5) */
	8,	/* no-radio-resources-available(8) */
	10,	/* normal-release(10) */
	6,	/* not-supported-qci-Value(6) */
	9,	/* procedure-cancelled(9) */
	16,	/* release-due-to-pre-emption(16) */
	14,	/* resources-not-available-for-the-slice(14) */
	12,	/* rl-failure-others(12) */
	1,	/* rl-failure-rlc(1) */
	13,	/* ue-rejection(13) */
	2,	/* unknown-or-already-allocated-gnb-cu-ue-f1ap-id(2) */
	3,	/* unknown-or-already-allocated-gnd-du-ue-f1ap-id(3) */
	4,	/* unknown-or-inconsistent-pair-of-ue-f1ap-id(4) */
	0	/* unspecified(0) */
	/* This list is extensible */
};
const asn_INTEGER_specifics_t asn_SPC_CauseRadioNetwork_specs_1 = {
	asn_MAP_CauseRadioNetwork_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_CauseRadioNetwork_enum2value_1,	/* N => "tag"; sorted by N */
	17,	/* Number of elements in the maps */
	12,	/* Extensions before this member */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_CauseRadioNetwork_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_CauseRadioNetwork = {
	"CauseRadioNetwork",
	"CauseRadioNetwork",
	&asn_OP_NativeEnumerated,
	asn_DEF_CauseRadioNetwork_tags_1,
	sizeof(asn_DEF_CauseRadioNetwork_tags_1)
		/sizeof(asn_DEF_CauseRadioNetwork_tags_1[0]), /* 1 */
	asn_DEF_CauseRadioNetwork_tags_1,	/* Same as above */
	sizeof(asn_DEF_CauseRadioNetwork_tags_1)
		/sizeof(asn_DEF_CauseRadioNetwork_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_CauseRadioNetwork_constr_1,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_CauseRadioNetwork_specs_1	/* Additional specs */
};

