/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "PUSCH-TPC-CommandConfig.h"

static int
memb_tpc_Index_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 15)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_tpc_IndexSUL_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 15)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_tpc_Index_constr_2 CC_NOTUSED = {
	{ 1, 1 }	/* (1..15) */,
	-1};
static asn_per_constraints_t asn_PER_memb_tpc_Index_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  1,  15 }	/* (1..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_tpc_IndexSUL_constr_3 CC_NOTUSED = {
	{ 1, 1 }	/* (1..15) */,
	-1};
static asn_per_constraints_t asn_PER_memb_tpc_IndexSUL_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  1,  15 }	/* (1..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_PUSCH_TPC_CommandConfig_1[] = {
	{ ATF_POINTER, 3, offsetof(struct PUSCH_TPC_CommandConfig, tpc_Index),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_tpc_Index_constr_2, &asn_PER_memb_tpc_Index_constr_2,  memb_tpc_Index_constraint_1 },
		0, 0, /* No default value */
		"tpc-Index"
		},
	{ ATF_POINTER, 2, offsetof(struct PUSCH_TPC_CommandConfig, tpc_IndexSUL),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_tpc_IndexSUL_constr_3, &asn_PER_memb_tpc_IndexSUL_constr_3,  memb_tpc_IndexSUL_constraint_1 },
		0, 0, /* No default value */
		"tpc-IndexSUL"
		},
	{ ATF_POINTER, 1, offsetof(struct PUSCH_TPC_CommandConfig, targetCell),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ServCellIndex,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"targetCell"
		},
};
static const int asn_MAP_PUSCH_TPC_CommandConfig_oms_1[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_PUSCH_TPC_CommandConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PUSCH_TPC_CommandConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* tpc-Index */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* tpc-IndexSUL */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* targetCell */
};
asn_SEQUENCE_specifics_t asn_SPC_PUSCH_TPC_CommandConfig_specs_1 = {
	sizeof(struct PUSCH_TPC_CommandConfig),
	offsetof(struct PUSCH_TPC_CommandConfig, _asn_ctx),
	asn_MAP_PUSCH_TPC_CommandConfig_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_PUSCH_TPC_CommandConfig_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_PUSCH_TPC_CommandConfig = {
	"PUSCH-TPC-CommandConfig",
	"PUSCH-TPC-CommandConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_PUSCH_TPC_CommandConfig_tags_1,
	sizeof(asn_DEF_PUSCH_TPC_CommandConfig_tags_1)
		/sizeof(asn_DEF_PUSCH_TPC_CommandConfig_tags_1[0]), /* 1 */
	asn_DEF_PUSCH_TPC_CommandConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_PUSCH_TPC_CommandConfig_tags_1)
		/sizeof(asn_DEF_PUSCH_TPC_CommandConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_PUSCH_TPC_CommandConfig_1,
	3,	/* Elements count */
	&asn_SPC_PUSCH_TPC_CommandConfig_specs_1	/* Additional specs */
};

