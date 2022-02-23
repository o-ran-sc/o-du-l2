/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "ReportConfigNR.h"

#include "PeriodicalReportConfig.h"
#include "EventTriggerConfig.h"
#include "ReportCGI.h"
static asn_oer_constraints_t asn_OER_type_reportType_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_reportType_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  1,  1,  0,  1 }	/* (0..1,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_reportType_2[] = {
	{ ATF_POINTER, 0, offsetof(struct ReportConfigNR__reportType, choice.periodical),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PeriodicalReportConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"periodical"
		},
	{ ATF_POINTER, 0, offsetof(struct ReportConfigNR__reportType, choice.eventTriggered),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EventTriggerConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eventTriggered"
		},
	{ ATF_POINTER, 0, offsetof(struct ReportConfigNR__reportType, choice.reportCGI),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReportCGI,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportCGI"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_reportType_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* periodical */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* eventTriggered */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* reportCGI */
};
static asn_CHOICE_specifics_t asn_SPC_reportType_specs_2 = {
	sizeof(struct ReportConfigNR__reportType),
	offsetof(struct ReportConfigNR__reportType, _asn_ctx),
	offsetof(struct ReportConfigNR__reportType, present),
	sizeof(((struct ReportConfigNR__reportType *)0)->present),
	asn_MAP_reportType_tag2el_2,
	3,	/* Count of tags in the map */
	0, 0,
	2	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_reportType_2 = {
	"reportType",
	"reportType",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_reportType_constr_2, &asn_PER_type_reportType_constr_2, CHOICE_constraint },
	asn_MBR_reportType_2,
	3,	/* Elements count */
	&asn_SPC_reportType_specs_2	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_ReportConfigNR_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ReportConfigNR, reportType),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_reportType_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportType"
		},
};
static const ber_tlv_tag_t asn_DEF_ReportConfigNR_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ReportConfigNR_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* reportType */
};
asn_SEQUENCE_specifics_t asn_SPC_ReportConfigNR_specs_1 = {
	sizeof(struct ReportConfigNR),
	offsetof(struct ReportConfigNR, _asn_ctx),
	asn_MAP_ReportConfigNR_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_ReportConfigNR = {
	"ReportConfigNR",
	"ReportConfigNR",
	&asn_OP_SEQUENCE,
	asn_DEF_ReportConfigNR_tags_1,
	sizeof(asn_DEF_ReportConfigNR_tags_1)
		/sizeof(asn_DEF_ReportConfigNR_tags_1[0]), /* 1 */
	asn_DEF_ReportConfigNR_tags_1,	/* Same as above */
	sizeof(asn_DEF_ReportConfigNR_tags_1)
		/sizeof(asn_DEF_ReportConfigNR_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_ReportConfigNR_1,
	1,	/* Elements count */
	&asn_SPC_ReportConfigNR_specs_1	/* Additional specs */
};

