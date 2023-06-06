/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-CommonDataTypes"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "ProtocolIE-IDE2.h"

int
ProtocolIE_IDE2_constraint(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 65535)) {
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
static asn_oer_constraints_t asn_OER_type_ProtocolIE_IDE2_constr_1 CC_NOTUSED = {
	{ 2, 1 }	/* (0..65535) */,
	-1};
asn_per_constraints_t asn_PER_type_ProtocolIE_IDE2_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 16,  16,  0,  65535 }	/* (0..65535) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const ber_tlv_tag_t asn_DEF_ProtocolIE_IDE2_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (2 << 2))
};
asn_TYPE_descriptor_t asn_DEF_ProtocolIE_IDE2 = {
	"ProtocolIE-IDE2",
	"ProtocolIE-IDE2",
	&asn_OP_NativeInteger,
	asn_DEF_ProtocolIE_IDE2_tags_1,
	sizeof(asn_DEF_ProtocolIE_IDE2_tags_1)
		/sizeof(asn_DEF_ProtocolIE_IDE2_tags_1[0]), /* 1 */
	asn_DEF_ProtocolIE_IDE2_tags_1,	/* Same as above */
	sizeof(asn_DEF_ProtocolIE_IDE2_tags_1)
		/sizeof(asn_DEF_ProtocolIE_IDE2_tags_1[0]), /* 1 */
	{ &asn_OER_type_ProtocolIE_IDE2_constr_1, &asn_PER_type_ProtocolIE_IDE2_constr_1, ProtocolIE_IDE2_constraint },
	0, 0,	/* No members */
	0	/* No specifics */
};

