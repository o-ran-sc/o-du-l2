/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "InterRAT-Parameters.h"

#include "EUTRA-Parameters.h"
static asn_TYPE_member_t asn_MBR_InterRAT_Parameters_1[] = {
	{ ATF_POINTER, 1, offsetof(struct InterRAT_Parameters, eutra),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EUTRA_Parameters,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eutra"
		},
};
static const int asn_MAP_InterRAT_Parameters_oms_1[] = { 0 };
static const ber_tlv_tag_t asn_DEF_InterRAT_Parameters_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_InterRAT_Parameters_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* eutra */
};
static asn_SEQUENCE_specifics_t asn_SPC_InterRAT_Parameters_specs_1 = {
	sizeof(struct InterRAT_Parameters),
	offsetof(struct InterRAT_Parameters, _asn_ctx),
	asn_MAP_InterRAT_Parameters_tag2el_1,
	1,	/* Count of tags in the map */
	asn_MAP_InterRAT_Parameters_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_InterRAT_Parameters = {
	"InterRAT-Parameters",
	"InterRAT-Parameters",
	&asn_OP_SEQUENCE,
	asn_DEF_InterRAT_Parameters_tags_1,
	sizeof(asn_DEF_InterRAT_Parameters_tags_1)
		/sizeof(asn_DEF_InterRAT_Parameters_tags_1[0]), /* 1 */
	asn_DEF_InterRAT_Parameters_tags_1,	/* Same as above */
	sizeof(asn_DEF_InterRAT_Parameters_tags_1)
		/sizeof(asn_DEF_InterRAT_Parameters_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_InterRAT_Parameters_1,
	1,	/* Elements count */
	&asn_SPC_InterRAT_Parameters_specs_1	/* Additional specs */
};

