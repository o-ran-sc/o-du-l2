/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "UL-DCCH-MessageType.h"

#include "MeasurementReport.h"
#include "RRCReconfigurationComplete.h"
#include "RRCSetupComplete.h"
#include "RRCReestablishmentComplete.h"
#include "RRCResumeComplete.h"
#include "SecurityModeComplete.h"
#include "SecurityModeFailure.h"
#include "ULInformationTransfer.h"
#include "LocationMeasurementIndication.h"
#include "UECapabilityInformation.h"
#include "CounterCheckResponse.h"
#include "UEAssistanceInformation.h"
#include "FailureInformation.h"
static asn_oer_constraints_t asn_OER_type_c1_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_c1_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  15 }	/* (0..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_UL_DCCH_MessageType_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_UL_DCCH_MessageType_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_c1_2[] = {
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.measurementReport),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasurementReport,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"measurementReport"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.rrcReconfigurationComplete),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RRCReconfigurationComplete,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rrcReconfigurationComplete"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.rrcSetupComplete),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RRCSetupComplete,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rrcSetupComplete"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.rrcReestablishmentComplete),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RRCReestablishmentComplete,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rrcReestablishmentComplete"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.rrcResumeComplete),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RRCResumeComplete,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rrcResumeComplete"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.securityModeComplete),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SecurityModeComplete,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"securityModeComplete"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.securityModeFailure),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SecurityModeFailure,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"securityModeFailure"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.ulInformationTransfer),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ULInformationTransfer,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ulInformationTransfer"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.locationMeasurementIndication),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LocationMeasurementIndication,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"locationMeasurementIndication"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.ueCapabilityInformation),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UECapabilityInformation,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ueCapabilityInformation"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.counterCheckResponse),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CounterCheckResponse,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"counterCheckResponse"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.ueAssistanceInformation),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UEAssistanceInformation,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ueAssistanceInformation"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.failureInformation),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FailureInformation,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"failureInformation"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.spare3),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spare3"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.spare2),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spare2"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.spare1),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spare1"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_c1_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* measurementReport */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* rrcReconfigurationComplete */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* rrcSetupComplete */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* rrcReestablishmentComplete */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* rrcResumeComplete */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* securityModeComplete */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* securityModeFailure */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* ulInformationTransfer */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* locationMeasurementIndication */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* ueCapabilityInformation */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* counterCheckResponse */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* ueAssistanceInformation */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* failureInformation */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* spare3 */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* spare2 */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 } /* spare1 */
};
static asn_CHOICE_specifics_t asn_SPC_c1_specs_2 = {
	sizeof(struct UL_DCCH_MessageType__c1),
	offsetof(struct UL_DCCH_MessageType__c1, _asn_ctx),
	offsetof(struct UL_DCCH_MessageType__c1, present),
	sizeof(((struct UL_DCCH_MessageType__c1 *)0)->present),
	asn_MAP_c1_tag2el_2,
	16,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_c1_2 = {
	"c1",
	"c1",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_c1_constr_2, &asn_PER_type_c1_constr_2, CHOICE_constraint },
	asn_MBR_c1_2,
	16,	/* Elements count */
	&asn_SPC_c1_specs_2	/* Additional specs */
};

static const ber_tlv_tag_t asn_DEF_messageClassExtension_tags_19[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SEQUENCE_specifics_t asn_SPC_messageClassExtension_specs_19 = {
	sizeof(struct UL_DCCH_MessageType__messageClassExtension),
	offsetof(struct UL_DCCH_MessageType__messageClassExtension, _asn_ctx),
	0,	/* No top level tags */
	0,	/* No tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_messageClassExtension_19 = {
	"messageClassExtension",
	"messageClassExtension",
	&asn_OP_SEQUENCE,
	asn_DEF_messageClassExtension_tags_19,
	sizeof(asn_DEF_messageClassExtension_tags_19)
		/sizeof(asn_DEF_messageClassExtension_tags_19[0]) - 1, /* 1 */
	asn_DEF_messageClassExtension_tags_19,	/* Same as above */
	sizeof(asn_DEF_messageClassExtension_tags_19)
		/sizeof(asn_DEF_messageClassExtension_tags_19[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	0, 0,	/* No members */
	&asn_SPC_messageClassExtension_specs_19	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_UL_DCCH_MessageType_1[] = {
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType, choice.c1),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_c1_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"c1"
		},
	{ ATF_POINTER, 0, offsetof(struct UL_DCCH_MessageType, choice.messageClassExtension),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_messageClassExtension_19,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"messageClassExtension"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_UL_DCCH_MessageType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* c1 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* messageClassExtension */
};
asn_CHOICE_specifics_t asn_SPC_UL_DCCH_MessageType_specs_1 = {
	sizeof(struct UL_DCCH_MessageType),
	offsetof(struct UL_DCCH_MessageType, _asn_ctx),
	offsetof(struct UL_DCCH_MessageType, present),
	sizeof(((struct UL_DCCH_MessageType *)0)->present),
	asn_MAP_UL_DCCH_MessageType_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_UL_DCCH_MessageType = {
	"UL-DCCH-MessageType",
	"UL-DCCH-MessageType",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_UL_DCCH_MessageType_constr_1, &asn_PER_type_UL_DCCH_MessageType_constr_1, CHOICE_constraint },
	asn_MBR_UL_DCCH_MessageType_1,
	2,	/* Elements count */
	&asn_SPC_UL_DCCH_MessageType_specs_1	/* Additional specs */
};

