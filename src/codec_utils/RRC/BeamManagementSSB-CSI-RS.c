/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "BeamManagementSSB-CSI-RS.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_maxNumberSSB_CSI_RS_ResourceOneTx_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_maxNumberSSB_CSI_RS_ResourceOneTx_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  4 }	/* (0..4) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_maxNumberCSI_RS_ResourceTwoTx_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_maxNumberCSI_RS_ResourceTwoTx_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  5 }	/* (0..5) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_supportedCSI_RS_Density_constr_15 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_supportedCSI_RS_Density_constr_15 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_maxNumberSSB_CSI_RS_ResourceOneTx_value2enum_2[] = {
	{ 0,	2,	"n0" },
	{ 1,	2,	"n8" },
	{ 2,	3,	"n16" },
	{ 3,	3,	"n32" },
	{ 4,	3,	"n64" }
};
static const unsigned int asn_MAP_maxNumberSSB_CSI_RS_ResourceOneTx_enum2value_2[] = {
	0,	/* n0(0) */
	2,	/* n16(2) */
	3,	/* n32(3) */
	4,	/* n64(4) */
	1	/* n8(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_maxNumberSSB_CSI_RS_ResourceOneTx_specs_2 = {
	asn_MAP_maxNumberSSB_CSI_RS_ResourceOneTx_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_maxNumberSSB_CSI_RS_ResourceOneTx_enum2value_2,	/* N => "tag"; sorted by N */
	5,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_maxNumberSSB_CSI_RS_ResourceOneTx_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_maxNumberSSB_CSI_RS_ResourceOneTx_2 = {
	"maxNumberSSB-CSI-RS-ResourceOneTx",
	"maxNumberSSB-CSI-RS-ResourceOneTx",
	&asn_OP_NativeEnumerated,
	asn_DEF_maxNumberSSB_CSI_RS_ResourceOneTx_tags_2,
	sizeof(asn_DEF_maxNumberSSB_CSI_RS_ResourceOneTx_tags_2)
		/sizeof(asn_DEF_maxNumberSSB_CSI_RS_ResourceOneTx_tags_2[0]) - 1, /* 1 */
	asn_DEF_maxNumberSSB_CSI_RS_ResourceOneTx_tags_2,	/* Same as above */
	sizeof(asn_DEF_maxNumberSSB_CSI_RS_ResourceOneTx_tags_2)
		/sizeof(asn_DEF_maxNumberSSB_CSI_RS_ResourceOneTx_tags_2[0]), /* 2 */
	{ &asn_OER_type_maxNumberSSB_CSI_RS_ResourceOneTx_constr_2, &asn_PER_type_maxNumberSSB_CSI_RS_ResourceOneTx_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_maxNumberSSB_CSI_RS_ResourceOneTx_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_maxNumberCSI_RS_ResourceTwoTx_value2enum_8[] = {
	{ 0,	2,	"n0" },
	{ 1,	2,	"n4" },
	{ 2,	2,	"n8" },
	{ 3,	3,	"n16" },
	{ 4,	3,	"n32" },
	{ 5,	3,	"n64" }
};
static const unsigned int asn_MAP_maxNumberCSI_RS_ResourceTwoTx_enum2value_8[] = {
	0,	/* n0(0) */
	3,	/* n16(3) */
	4,	/* n32(4) */
	1,	/* n4(1) */
	5,	/* n64(5) */
	2	/* n8(2) */
};
static const asn_INTEGER_specifics_t asn_SPC_maxNumberCSI_RS_ResourceTwoTx_specs_8 = {
	asn_MAP_maxNumberCSI_RS_ResourceTwoTx_value2enum_8,	/* "tag" => N; sorted by tag */
	asn_MAP_maxNumberCSI_RS_ResourceTwoTx_enum2value_8,	/* N => "tag"; sorted by N */
	6,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_maxNumberCSI_RS_ResourceTwoTx_tags_8[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_maxNumberCSI_RS_ResourceTwoTx_8 = {
	"maxNumberCSI-RS-ResourceTwoTx",
	"maxNumberCSI-RS-ResourceTwoTx",
	&asn_OP_NativeEnumerated,
	asn_DEF_maxNumberCSI_RS_ResourceTwoTx_tags_8,
	sizeof(asn_DEF_maxNumberCSI_RS_ResourceTwoTx_tags_8)
		/sizeof(asn_DEF_maxNumberCSI_RS_ResourceTwoTx_tags_8[0]) - 1, /* 1 */
	asn_DEF_maxNumberCSI_RS_ResourceTwoTx_tags_8,	/* Same as above */
	sizeof(asn_DEF_maxNumberCSI_RS_ResourceTwoTx_tags_8)
		/sizeof(asn_DEF_maxNumberCSI_RS_ResourceTwoTx_tags_8[0]), /* 2 */
	{ &asn_OER_type_maxNumberCSI_RS_ResourceTwoTx_constr_8, &asn_PER_type_maxNumberCSI_RS_ResourceTwoTx_constr_8, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_maxNumberCSI_RS_ResourceTwoTx_specs_8	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_supportedCSI_RS_Density_value2enum_15[] = {
	{ 0,	3,	"one" },
	{ 1,	5,	"three" },
	{ 2,	11,	"oneAndThree" }
};
static const unsigned int asn_MAP_supportedCSI_RS_Density_enum2value_15[] = {
	0,	/* one(0) */
	2,	/* oneAndThree(2) */
	1	/* three(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_supportedCSI_RS_Density_specs_15 = {
	asn_MAP_supportedCSI_RS_Density_value2enum_15,	/* "tag" => N; sorted by tag */
	asn_MAP_supportedCSI_RS_Density_enum2value_15,	/* N => "tag"; sorted by N */
	3,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_supportedCSI_RS_Density_tags_15[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_supportedCSI_RS_Density_15 = {
	"supportedCSI-RS-Density",
	"supportedCSI-RS-Density",
	&asn_OP_NativeEnumerated,
	asn_DEF_supportedCSI_RS_Density_tags_15,
	sizeof(asn_DEF_supportedCSI_RS_Density_tags_15)
		/sizeof(asn_DEF_supportedCSI_RS_Density_tags_15[0]) - 1, /* 1 */
	asn_DEF_supportedCSI_RS_Density_tags_15,	/* Same as above */
	sizeof(asn_DEF_supportedCSI_RS_Density_tags_15)
		/sizeof(asn_DEF_supportedCSI_RS_Density_tags_15[0]), /* 2 */
	{ &asn_OER_type_supportedCSI_RS_Density_constr_15, &asn_PER_type_supportedCSI_RS_Density_constr_15, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_supportedCSI_RS_Density_specs_15	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_BeamManagementSSB_CSI_RS_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct BeamManagementSSB_CSI_RS, maxNumberSSB_CSI_RS_ResourceOneTx),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_maxNumberSSB_CSI_RS_ResourceOneTx_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxNumberSSB-CSI-RS-ResourceOneTx"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BeamManagementSSB_CSI_RS, maxNumberCSI_RS_ResourceTwoTx),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_maxNumberCSI_RS_ResourceTwoTx_8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxNumberCSI-RS-ResourceTwoTx"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BeamManagementSSB_CSI_RS, supportedCSI_RS_Density),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_supportedCSI_RS_Density_15,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"supportedCSI-RS-Density"
		},
};
static const ber_tlv_tag_t asn_DEF_BeamManagementSSB_CSI_RS_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_BeamManagementSSB_CSI_RS_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* maxNumberSSB-CSI-RS-ResourceOneTx */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* maxNumberCSI-RS-ResourceTwoTx */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* supportedCSI-RS-Density */
};
asn_SEQUENCE_specifics_t asn_SPC_BeamManagementSSB_CSI_RS_specs_1 = {
	sizeof(struct BeamManagementSSB_CSI_RS),
	offsetof(struct BeamManagementSSB_CSI_RS, _asn_ctx),
	asn_MAP_BeamManagementSSB_CSI_RS_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_BeamManagementSSB_CSI_RS = {
	"BeamManagementSSB-CSI-RS",
	"BeamManagementSSB-CSI-RS",
	&asn_OP_SEQUENCE,
	asn_DEF_BeamManagementSSB_CSI_RS_tags_1,
	sizeof(asn_DEF_BeamManagementSSB_CSI_RS_tags_1)
		/sizeof(asn_DEF_BeamManagementSSB_CSI_RS_tags_1[0]), /* 1 */
	asn_DEF_BeamManagementSSB_CSI_RS_tags_1,	/* Same as above */
	sizeof(asn_DEF_BeamManagementSSB_CSI_RS_tags_1)
		/sizeof(asn_DEF_BeamManagementSSB_CSI_RS_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_BeamManagementSSB_CSI_RS_1,
	3,	/* Elements count */
	&asn_SPC_BeamManagementSSB_CSI_RS_specs_1	/* Additional specs */
};

