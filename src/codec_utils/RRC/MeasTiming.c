/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "MeasTiming.h"

#include "SS-RSSI-Measurement.h"
static asn_TYPE_member_t asn_MBR_frequencyAndTiming_2[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct MeasTiming__frequencyAndTiming, carrierFreq),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ARFCN_ValueNR,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"carrierFreq"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MeasTiming__frequencyAndTiming, ssbSubcarrierSpacing),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SubcarrierSpacing,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ssbSubcarrierSpacing"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MeasTiming__frequencyAndTiming, ssb_MeasurementTimingConfiguration),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SSB_MTC,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ssb-MeasurementTimingConfiguration"
		},
	{ ATF_POINTER, 1, offsetof(struct MeasTiming__frequencyAndTiming, ss_RSSI_Measurement),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SS_RSSI_Measurement,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ss-RSSI-Measurement"
		},
};
static const int asn_MAP_frequencyAndTiming_oms_2[] = { 3 };
static const ber_tlv_tag_t asn_DEF_frequencyAndTiming_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_frequencyAndTiming_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* carrierFreq */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* ssbSubcarrierSpacing */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* ssb-MeasurementTimingConfiguration */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* ss-RSSI-Measurement */
};
static asn_SEQUENCE_specifics_t asn_SPC_frequencyAndTiming_specs_2 = {
	sizeof(struct MeasTiming__frequencyAndTiming),
	offsetof(struct MeasTiming__frequencyAndTiming, _asn_ctx),
	asn_MAP_frequencyAndTiming_tag2el_2,
	4,	/* Count of tags in the map */
	asn_MAP_frequencyAndTiming_oms_2,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_frequencyAndTiming_2 = {
	"frequencyAndTiming",
	"frequencyAndTiming",
	&asn_OP_SEQUENCE,
	asn_DEF_frequencyAndTiming_tags_2,
	sizeof(asn_DEF_frequencyAndTiming_tags_2)
		/sizeof(asn_DEF_frequencyAndTiming_tags_2[0]) - 1, /* 1 */
	asn_DEF_frequencyAndTiming_tags_2,	/* Same as above */
	sizeof(asn_DEF_frequencyAndTiming_tags_2)
		/sizeof(asn_DEF_frequencyAndTiming_tags_2[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_frequencyAndTiming_2,
	4,	/* Elements count */
	&asn_SPC_frequencyAndTiming_specs_2	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_MeasTiming_1[] = {
	{ ATF_POINTER, 1, offsetof(struct MeasTiming, frequencyAndTiming),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_frequencyAndTiming_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"frequencyAndTiming"
		},
};
static const int asn_MAP_MeasTiming_oms_1[] = { 0 };
static const ber_tlv_tag_t asn_DEF_MeasTiming_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_MeasTiming_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* frequencyAndTiming */
};
asn_SEQUENCE_specifics_t asn_SPC_MeasTiming_specs_1 = {
	sizeof(struct MeasTiming),
	offsetof(struct MeasTiming, _asn_ctx),
	asn_MAP_MeasTiming_tag2el_1,
	1,	/* Count of tags in the map */
	asn_MAP_MeasTiming_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_MeasTiming = {
	"MeasTiming",
	"MeasTiming",
	&asn_OP_SEQUENCE,
	asn_DEF_MeasTiming_tags_1,
	sizeof(asn_DEF_MeasTiming_tags_1)
		/sizeof(asn_DEF_MeasTiming_tags_1[0]), /* 1 */
	asn_DEF_MeasTiming_tags_1,	/* Same as above */
	sizeof(asn_DEF_MeasTiming_tags_1)
		/sizeof(asn_DEF_MeasTiming_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_MeasTiming_1,
	1,	/* Elements count */
	&asn_SPC_MeasTiming_specs_1	/* Additional specs */
};

