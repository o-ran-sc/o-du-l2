/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "TCI-State.h"

#include "QCL-Info.h"
asn_TYPE_member_t asn_MBR_TCI_State_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TCI_State, tci_StateId),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TCI_StateId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"tci-StateId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TCI_State, qcl_Type1),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_QCL_Info,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"qcl-Type1"
		},
	{ ATF_POINTER, 1, offsetof(struct TCI_State, qcl_Type2),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_QCL_Info,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"qcl-Type2"
		},
};
static const int asn_MAP_TCI_State_oms_1[] = { 2 };
static const ber_tlv_tag_t asn_DEF_TCI_State_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_TCI_State_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* tci-StateId */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* qcl-Type1 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* qcl-Type2 */
};
asn_SEQUENCE_specifics_t asn_SPC_TCI_State_specs_1 = {
	sizeof(struct TCI_State),
	offsetof(struct TCI_State, _asn_ctx),
	asn_MAP_TCI_State_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_TCI_State_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_TCI_State = {
	"TCI-State",
	"TCI-State",
	&asn_OP_SEQUENCE,
	asn_DEF_TCI_State_tags_1,
	sizeof(asn_DEF_TCI_State_tags_1)
		/sizeof(asn_DEF_TCI_State_tags_1[0]), /* 1 */
	asn_DEF_TCI_State_tags_1,	/* Same as above */
	sizeof(asn_DEF_TCI_State_tags_1)
		/sizeof(asn_DEF_TCI_State_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_TCI_State_1,
	3,	/* Elements count */
	&asn_SPC_TCI_State_specs_1	/* Additional specs */
};

