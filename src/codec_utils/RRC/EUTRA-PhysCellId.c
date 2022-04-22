/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "EUTRA-PhysCellId.h"

int
EUTRA_PhysCellId_constraint(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 503)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

/*
 * This type is implemented using NativeInteger,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_EUTRA_PhysCellId_constr_1 CC_NOTUSED = {
	{ 2, 1 }	/* (0..503) */,
	-1};
asn_per_constraints_t asn_PER_type_EUTRA_PhysCellId_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9,  0,  503 }	/* (0..503) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const ber_tlv_tag_t asn_DEF_EUTRA_PhysCellId_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (2 << 2))
};
asn_TYPE_descriptor_t asn_DEF_EUTRA_PhysCellId = {
	"EUTRA-PhysCellId",
	"EUTRA-PhysCellId",
	&asn_OP_NativeInteger,
	asn_DEF_EUTRA_PhysCellId_tags_1,
	sizeof(asn_DEF_EUTRA_PhysCellId_tags_1)
		/sizeof(asn_DEF_EUTRA_PhysCellId_tags_1[0]), /* 1 */
	asn_DEF_EUTRA_PhysCellId_tags_1,	/* Same as above */
	sizeof(asn_DEF_EUTRA_PhysCellId_tags_1)
		/sizeof(asn_DEF_EUTRA_PhysCellId_tags_1[0]), /* 1 */
	{ &asn_OER_type_EUTRA_PhysCellId_constr_1, &asn_PER_type_EUTRA_PhysCellId_constr_1, EUTRA_PhysCellId_constraint },
	0, 0,	/* No members */
	0	/* No specifics */
};

