/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "EventTriggerConfigInterRAT.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_maxReportCells_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 8)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_eventId_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_eventId_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  1,  1,  0,  1 }	/* (0..1,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_reportAmount_constr_19 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_reportAmount_constr_19 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_maxReportCells_constr_29 CC_NOTUSED = {
	{ 1, 1 }	/* (1..8) */,
	-1};
static asn_per_constraints_t asn_PER_memb_maxReportCells_constr_29 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  1,  8 }	/* (1..8) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_eventB1_3[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT__eventId__eventB1, b1_ThresholdEUTRA),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_MeasTriggerQuantityEUTRA,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"b1-ThresholdEUTRA"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT__eventId__eventB1, reportOnLeave),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportOnLeave"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT__eventId__eventB1, hysteresis),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Hysteresis,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"hysteresis"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT__eventId__eventB1, timeToTrigger),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TimeToTrigger,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"timeToTrigger"
		},
};
static const ber_tlv_tag_t asn_DEF_eventB1_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_eventB1_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* b1-ThresholdEUTRA */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* reportOnLeave */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* hysteresis */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* timeToTrigger */
};
static asn_SEQUENCE_specifics_t asn_SPC_eventB1_specs_3 = {
	sizeof(struct EventTriggerConfigInterRAT__eventId__eventB1),
	offsetof(struct EventTriggerConfigInterRAT__eventId__eventB1, _asn_ctx),
	asn_MAP_eventB1_tag2el_3,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	4,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_eventB1_3 = {
	"eventB1",
	"eventB1",
	&asn_OP_SEQUENCE,
	asn_DEF_eventB1_tags_3,
	sizeof(asn_DEF_eventB1_tags_3)
		/sizeof(asn_DEF_eventB1_tags_3[0]) - 1, /* 1 */
	asn_DEF_eventB1_tags_3,	/* Same as above */
	sizeof(asn_DEF_eventB1_tags_3)
		/sizeof(asn_DEF_eventB1_tags_3[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_eventB1_3,
	4,	/* Elements count */
	&asn_SPC_eventB1_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_eventB2_9[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT__eventId__eventB2, b2_Threshold1),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_MeasTriggerQuantity,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"b2-Threshold1"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT__eventId__eventB2, b2_Threshold2EUTRA),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_MeasTriggerQuantityEUTRA,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"b2-Threshold2EUTRA"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT__eventId__eventB2, reportOnLeave),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportOnLeave"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT__eventId__eventB2, hysteresis),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Hysteresis,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"hysteresis"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT__eventId__eventB2, timeToTrigger),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TimeToTrigger,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"timeToTrigger"
		},
};
static const ber_tlv_tag_t asn_DEF_eventB2_tags_9[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_eventB2_tag2el_9[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* b2-Threshold1 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* b2-Threshold2EUTRA */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* reportOnLeave */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* hysteresis */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* timeToTrigger */
};
static asn_SEQUENCE_specifics_t asn_SPC_eventB2_specs_9 = {
	sizeof(struct EventTriggerConfigInterRAT__eventId__eventB2),
	offsetof(struct EventTriggerConfigInterRAT__eventId__eventB2, _asn_ctx),
	asn_MAP_eventB2_tag2el_9,
	5,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	5,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_eventB2_9 = {
	"eventB2",
	"eventB2",
	&asn_OP_SEQUENCE,
	asn_DEF_eventB2_tags_9,
	sizeof(asn_DEF_eventB2_tags_9)
		/sizeof(asn_DEF_eventB2_tags_9[0]) - 1, /* 1 */
	asn_DEF_eventB2_tags_9,	/* Same as above */
	sizeof(asn_DEF_eventB2_tags_9)
		/sizeof(asn_DEF_eventB2_tags_9[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_eventB2_9,
	5,	/* Elements count */
	&asn_SPC_eventB2_specs_9	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_eventId_2[] = {
	{ ATF_POINTER, 0, offsetof(struct EventTriggerConfigInterRAT__eventId, choice.eventB1),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_eventB1_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eventB1"
		},
	{ ATF_POINTER, 0, offsetof(struct EventTriggerConfigInterRAT__eventId, choice.eventB2),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_eventB2_9,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eventB2"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_eventId_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* eventB1 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* eventB2 */
};
static asn_CHOICE_specifics_t asn_SPC_eventId_specs_2 = {
	sizeof(struct EventTriggerConfigInterRAT__eventId),
	offsetof(struct EventTriggerConfigInterRAT__eventId, _asn_ctx),
	offsetof(struct EventTriggerConfigInterRAT__eventId, present),
	sizeof(((struct EventTriggerConfigInterRAT__eventId *)0)->present),
	asn_MAP_eventId_tag2el_2,
	2,	/* Count of tags in the map */
	0, 0,
	2	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_eventId_2 = {
	"eventId",
	"eventId",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_eventId_constr_2, &asn_PER_type_eventId_constr_2, CHOICE_constraint },
	asn_MBR_eventId_2,
	2,	/* Elements count */
	&asn_SPC_eventId_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_reportAmount_value2enum_19[] = {
	{ 0,	2,	"r1" },
	{ 1,	2,	"r2" },
	{ 2,	2,	"r4" },
	{ 3,	2,	"r8" },
	{ 4,	3,	"r16" },
	{ 5,	3,	"r32" },
	{ 6,	3,	"r64" },
	{ 7,	8,	"infinity" }
};
static const unsigned int asn_MAP_reportAmount_enum2value_19[] = {
	7,	/* infinity(7) */
	0,	/* r1(0) */
	4,	/* r16(4) */
	1,	/* r2(1) */
	5,	/* r32(5) */
	2,	/* r4(2) */
	6,	/* r64(6) */
	3	/* r8(3) */
};
static const asn_INTEGER_specifics_t asn_SPC_reportAmount_specs_19 = {
	asn_MAP_reportAmount_value2enum_19,	/* "tag" => N; sorted by tag */
	asn_MAP_reportAmount_enum2value_19,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_reportAmount_tags_19[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_reportAmount_19 = {
	"reportAmount",
	"reportAmount",
	&asn_OP_NativeEnumerated,
	asn_DEF_reportAmount_tags_19,
	sizeof(asn_DEF_reportAmount_tags_19)
		/sizeof(asn_DEF_reportAmount_tags_19[0]) - 1, /* 1 */
	asn_DEF_reportAmount_tags_19,	/* Same as above */
	sizeof(asn_DEF_reportAmount_tags_19)
		/sizeof(asn_DEF_reportAmount_tags_19[0]), /* 2 */
	{ &asn_OER_type_reportAmount_constr_19, &asn_PER_type_reportAmount_constr_19, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_reportAmount_specs_19	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_EventTriggerConfigInterRAT_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT, eventId),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_eventId_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eventId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT, rsType),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NR_RS_Type,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rsType"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT, reportInterval),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReportInterval,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportInterval"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT, reportAmount),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_reportAmount_19,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportAmount"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT, reportQuantity),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasReportQuantity,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportQuantity"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EventTriggerConfigInterRAT, maxReportCells),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_maxReportCells_constr_29, &asn_PER_memb_maxReportCells_constr_29,  memb_maxReportCells_constraint_1 },
		0, 0, /* No default value */
		"maxReportCells"
		},
};
static const ber_tlv_tag_t asn_DEF_EventTriggerConfigInterRAT_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_EventTriggerConfigInterRAT_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* eventId */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* rsType */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* reportInterval */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* reportAmount */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* reportQuantity */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* maxReportCells */
};
asn_SEQUENCE_specifics_t asn_SPC_EventTriggerConfigInterRAT_specs_1 = {
	sizeof(struct EventTriggerConfigInterRAT),
	offsetof(struct EventTriggerConfigInterRAT, _asn_ctx),
	asn_MAP_EventTriggerConfigInterRAT_tag2el_1,
	6,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	6,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_EventTriggerConfigInterRAT = {
	"EventTriggerConfigInterRAT",
	"EventTriggerConfigInterRAT",
	&asn_OP_SEQUENCE,
	asn_DEF_EventTriggerConfigInterRAT_tags_1,
	sizeof(asn_DEF_EventTriggerConfigInterRAT_tags_1)
		/sizeof(asn_DEF_EventTriggerConfigInterRAT_tags_1[0]), /* 1 */
	asn_DEF_EventTriggerConfigInterRAT_tags_1,	/* Same as above */
	sizeof(asn_DEF_EventTriggerConfigInterRAT_tags_1)
		/sizeof(asn_DEF_EventTriggerConfigInterRAT_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_EventTriggerConfigInterRAT_1,
	6,	/* Elements count */
	&asn_SPC_EventTriggerConfigInterRAT_specs_1	/* Additional specs */
};

