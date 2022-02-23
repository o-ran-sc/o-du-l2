/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "QuantityConfigRS.h"

asn_TYPE_member_t asn_MBR_QuantityConfigRS_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct QuantityConfigRS, ssb_FilterConfig),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FilterConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ssb-FilterConfig"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct QuantityConfigRS, csi_RS_FilterConfig),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FilterConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"csi-RS-FilterConfig"
		},
};
static const ber_tlv_tag_t asn_DEF_QuantityConfigRS_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_QuantityConfigRS_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ssb-FilterConfig */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* csi-RS-FilterConfig */
};
asn_SEQUENCE_specifics_t asn_SPC_QuantityConfigRS_specs_1 = {
	sizeof(struct QuantityConfigRS),
	offsetof(struct QuantityConfigRS, _asn_ctx),
	asn_MAP_QuantityConfigRS_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_QuantityConfigRS = {
	"QuantityConfigRS",
	"QuantityConfigRS",
	&asn_OP_SEQUENCE,
	asn_DEF_QuantityConfigRS_tags_1,
	sizeof(asn_DEF_QuantityConfigRS_tags_1)
		/sizeof(asn_DEF_QuantityConfigRS_tags_1[0]), /* 1 */
	asn_DEF_QuantityConfigRS_tags_1,	/* Same as above */
	sizeof(asn_DEF_QuantityConfigRS_tags_1)
		/sizeof(asn_DEF_QuantityConfigRS_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_QuantityConfigRS_1,
	2,	/* Elements count */
	&asn_SPC_QuantityConfigRS_specs_1	/* Additional specs */
};

