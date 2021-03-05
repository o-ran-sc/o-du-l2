/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "codeathon/E2SM_KPM.asn1"
 * 	`asn1c -D ./codeathon/e2smkpm/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "E2SM-KPM-IndicationHeader.h"

#include "E2SM-KPM-IndicationHeader-Format1.h"
static asn_oer_constraints_t asn_OER_type_E2SM_KPM_IndicationHeader_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_E2SM_KPM_IndicationHeader_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  0,  0,  0,  0 }	/* (0..0,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_E2SM_KPM_IndicationHeader_1[] = {
	{ ATF_POINTER, 0, offsetof(struct E2SM_KPM_IndicationHeader, choice.indicationHeader_Format1),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_E2SM_KPM_IndicationHeader_Format1,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"indicationHeader-Format1"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_E2SM_KPM_IndicationHeader_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* indicationHeader-Format1 */
};
static asn_CHOICE_specifics_t asn_SPC_E2SM_KPM_IndicationHeader_specs_1 = {
	sizeof(struct E2SM_KPM_IndicationHeader),
	offsetof(struct E2SM_KPM_IndicationHeader, _asn_ctx),
	offsetof(struct E2SM_KPM_IndicationHeader, present),
	sizeof(((struct E2SM_KPM_IndicationHeader *)0)->present),
	asn_MAP_E2SM_KPM_IndicationHeader_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0,
	1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_E2SM_KPM_IndicationHeader = {
	"E2SM-KPM-IndicationHeader",
	"E2SM-KPM-IndicationHeader",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_E2SM_KPM_IndicationHeader_constr_1, &asn_PER_type_E2SM_KPM_IndicationHeader_constr_1, CHOICE_constraint },
	asn_MBR_E2SM_KPM_IndicationHeader_1,
	1,	/* Elements count */
	&asn_SPC_E2SM_KPM_IndicationHeader_specs_1	/* Additional specs */
};

