/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "UE-MRDC-CapabilityAddFRX-Mode.h"

asn_TYPE_member_t asn_MBR_UE_MRDC_CapabilityAddFRX_Mode_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UE_MRDC_CapabilityAddFRX_Mode, measAndMobParametersMRDC_FRX_Diff),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasAndMobParametersMRDC_FRX_Diff,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"measAndMobParametersMRDC-FRX-Diff"
		},
};
static const ber_tlv_tag_t asn_DEF_UE_MRDC_CapabilityAddFRX_Mode_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UE_MRDC_CapabilityAddFRX_Mode_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* measAndMobParametersMRDC-FRX-Diff */
};
asn_SEQUENCE_specifics_t asn_SPC_UE_MRDC_CapabilityAddFRX_Mode_specs_1 = {
	sizeof(struct UE_MRDC_CapabilityAddFRX_Mode),
	offsetof(struct UE_MRDC_CapabilityAddFRX_Mode, _asn_ctx),
	asn_MAP_UE_MRDC_CapabilityAddFRX_Mode_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_UE_MRDC_CapabilityAddFRX_Mode = {
	"UE-MRDC-CapabilityAddFRX-Mode",
	"UE-MRDC-CapabilityAddFRX-Mode",
	&asn_OP_SEQUENCE,
	asn_DEF_UE_MRDC_CapabilityAddFRX_Mode_tags_1,
	sizeof(asn_DEF_UE_MRDC_CapabilityAddFRX_Mode_tags_1)
		/sizeof(asn_DEF_UE_MRDC_CapabilityAddFRX_Mode_tags_1[0]), /* 1 */
	asn_DEF_UE_MRDC_CapabilityAddFRX_Mode_tags_1,	/* Same as above */
	sizeof(asn_DEF_UE_MRDC_CapabilityAddFRX_Mode_tags_1)
		/sizeof(asn_DEF_UE_MRDC_CapabilityAddFRX_Mode_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_UE_MRDC_CapabilityAddFRX_Mode_1,
	1,	/* Elements count */
	&asn_SPC_UE_MRDC_CapabilityAddFRX_Mode_specs_1	/* Additional specs */
};

