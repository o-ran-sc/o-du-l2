/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Contents"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "ErrorIndication.h"

asn_TYPE_member_t asn_MBR_ErrorIndication_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ErrorIndication, protocolIEs),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolIE_Container_4587P2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"protocolIEs"
		},
};
static const ber_tlv_tag_t asn_DEF_ErrorIndication_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ErrorIndication_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* protocolIEs */
};
asn_SEQUENCE_specifics_t asn_SPC_ErrorIndication_specs_1 = {
	sizeof(struct ErrorIndication),
	offsetof(struct ErrorIndication, _asn_ctx),
	asn_MAP_ErrorIndication_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_ErrorIndication = {
	"ErrorIndication",
	"ErrorIndication",
	&asn_OP_SEQUENCE,
	asn_DEF_ErrorIndication_tags_1,
	sizeof(asn_DEF_ErrorIndication_tags_1)
		/sizeof(asn_DEF_ErrorIndication_tags_1[0]), /* 1 */
	asn_DEF_ErrorIndication_tags_1,	/* Same as above */
	sizeof(asn_DEF_ErrorIndication_tags_1)
		/sizeof(asn_DEF_ErrorIndication_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_ErrorIndication_1,
	1,	/* Elements count */
	&asn_SPC_ErrorIndication_specs_1	/* Additional specs */
};

