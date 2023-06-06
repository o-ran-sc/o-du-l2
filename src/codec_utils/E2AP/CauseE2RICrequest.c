/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "CauseE2RICrequest.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_CauseE2RICrequest_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_CauseE2RICrequest_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  4,  4,  0,  13 }	/* (0..13,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_CauseE2RICrequest_value2enum_1[] = {
	{ 0,	23,	"ran-function-id-invalid" },
	{ 1,	20,	"action-not-supported" },
	{ 2,	17,	"excessive-actions" },
	{ 3,	16,	"duplicate-action" },
	{ 4,	23,	"duplicate-event-trigger" },
	{ 5,	23,	"function-resource-limit" },
	{ 6,	18,	"request-id-unknown" },
	{ 7,	46,	"inconsistent-action-subsequent-action-sequence" },
	{ 8,	23,	"control-message-invalid" },
	{ 9,	27,	"ric-call-process-id-invalid" },
	{ 10,	21,	"control-timer-expired" },
	{ 11,	25,	"control-failed-to-execute" },
	{ 12,	16,	"system-not-ready" },
	{ 13,	11,	"unspecified" },
	{ 14,	33,	"ric-subscription-end-time-expired" },
	{ 15,	33,	"ric-subscription-end-time-invalid" },
	{ 16,	24,	"duplicate-ric-request-id" },
	{ 17,	24,	"eventTriggerNotSupported" },
	{ 18,	33,	"requested-information-unavailable" },
	{ 19,	27,	"invalid-information-request" }
	/* This list is extensible */
};
static const unsigned int asn_MAP_CauseE2RICrequest_enum2value_1[] = {
	1,	/* action-not-supported(1) */
	11,	/* control-failed-to-execute(11) */
	8,	/* control-message-invalid(8) */
	10,	/* control-timer-expired(10) */
	3,	/* duplicate-action(3) */
	4,	/* duplicate-event-trigger(4) */
	16,	/* duplicate-ric-request-id(16) */
	17,	/* eventTriggerNotSupported(17) */
	2,	/* excessive-actions(2) */
	5,	/* function-resource-limit(5) */
	7,	/* inconsistent-action-subsequent-action-sequence(7) */
	19,	/* invalid-information-request(19) */
	0,	/* ran-function-id-invalid(0) */
	6,	/* request-id-unknown(6) */
	18,	/* requested-information-unavailable(18) */
	9,	/* ric-call-process-id-invalid(9) */
	14,	/* ric-subscription-end-time-expired(14) */
	15,	/* ric-subscription-end-time-invalid(15) */
	12,	/* system-not-ready(12) */
	13	/* unspecified(13) */
	/* This list is extensible */
};
const asn_INTEGER_specifics_t asn_SPC_CauseE2RICrequest_specs_1 = {
	asn_MAP_CauseE2RICrequest_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_CauseE2RICrequest_enum2value_1,	/* N => "tag"; sorted by N */
	20,	/* Number of elements in the maps */
	15,	/* Extensions before this member */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_CauseE2RICrequest_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_CauseE2RICrequest = {
	"CauseE2RICrequest",
	"CauseE2RICrequest",
	&asn_OP_NativeEnumerated,
	asn_DEF_CauseE2RICrequest_tags_1,
	sizeof(asn_DEF_CauseE2RICrequest_tags_1)
		/sizeof(asn_DEF_CauseE2RICrequest_tags_1[0]), /* 1 */
	asn_DEF_CauseE2RICrequest_tags_1,	/* Same as above */
	sizeof(asn_DEF_CauseE2RICrequest_tags_1)
		/sizeof(asn_DEF_CauseE2RICrequest_tags_1[0]), /* 1 */
	{ &asn_OER_type_CauseE2RICrequest_constr_1, &asn_PER_type_CauseE2RICrequest_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_CauseE2RICrequest_specs_1	/* Additional specs */
};

