/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "E2nodeComponentInterfaceNG.h"

asn_TYPE_member_t asn_MBR_E2nodeComponentInterfaceNG_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct E2nodeComponentInterfaceNG, amf_name),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AMFName,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"amf-name"
		},
};
static const ber_tlv_tag_t asn_DEF_E2nodeComponentInterfaceNG_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_E2nodeComponentInterfaceNG_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* amf-name */
};
asn_SEQUENCE_specifics_t asn_SPC_E2nodeComponentInterfaceNG_specs_1 = {
	sizeof(struct E2nodeComponentInterfaceNG),
	offsetof(struct E2nodeComponentInterfaceNG, _asn_ctx),
	asn_MAP_E2nodeComponentInterfaceNG_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_E2nodeComponentInterfaceNG = {
	"E2nodeComponentInterfaceNG",
	"E2nodeComponentInterfaceNG",
	&asn_OP_SEQUENCE,
	asn_DEF_E2nodeComponentInterfaceNG_tags_1,
	sizeof(asn_DEF_E2nodeComponentInterfaceNG_tags_1)
		/sizeof(asn_DEF_E2nodeComponentInterfaceNG_tags_1[0]), /* 1 */
	asn_DEF_E2nodeComponentInterfaceNG_tags_1,	/* Same as above */
	sizeof(asn_DEF_E2nodeComponentInterfaceNG_tags_1)
		/sizeof(asn_DEF_E2nodeComponentInterfaceNG_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_E2nodeComponentInterfaceNG_1,
	1,	/* Elements count */
	&asn_SPC_E2nodeComponentInterfaceNG_specs_1	/* Additional specs */
};

