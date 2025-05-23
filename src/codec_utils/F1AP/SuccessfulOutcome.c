/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Descriptions"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#include "SuccessfulOutcome.h"

static const long asn_VAL_1_id_Reset = 0;
static const long asn_VAL_1_reject = 0;
static const long asn_VAL_2_id_F1Setup = 1;
static const long asn_VAL_2_reject = 0;
static const long asn_VAL_3_id_gNBDUConfigurationUpdate = 3;
static const long asn_VAL_3_reject = 0;
static const long asn_VAL_4_id_gNBCUConfigurationUpdate = 4;
static const long asn_VAL_4_reject = 0;
static const long asn_VAL_5_id_UEContextSetup = 5;
static const long asn_VAL_5_reject = 0;
static const long asn_VAL_6_id_UEContextRelease = 6;
static const long asn_VAL_6_reject = 0;
static const long asn_VAL_7_id_UEContextModification = 7;
static const long asn_VAL_7_reject = 0;
static const long asn_VAL_8_id_UEContextModificationRequired = 8;
static const long asn_VAL_8_reject = 0;
static const long asn_VAL_9_id_WriteReplaceWarning = 20;
static const long asn_VAL_9_reject = 0;
static const long asn_VAL_10_id_PWSCancel = 21;
static const long asn_VAL_10_reject = 0;
static const long asn_VAL_11_id_GNBDUResourceCoordination = 16;
static const long asn_VAL_11_reject = 0;
static const long asn_VAL_12_id_ErrorIndication = 2;
static const long asn_VAL_12_ignore = 1;
static const long asn_VAL_13_id_UEContextReleaseRequest = 10;
static const long asn_VAL_13_ignore = 1;
static const long asn_VAL_14_id_DLRRCMessageTransfer = 12;
static const long asn_VAL_14_ignore = 1;
static const long asn_VAL_15_id_ULRRCMessageTransfer = 13;
static const long asn_VAL_15_ignore = 1;
static const long asn_VAL_16_id_UEInactivityNotification = 15;
static const long asn_VAL_16_ignore = 1;
static const long asn_VAL_17_id_privateMessage = 14;
static const long asn_VAL_17_ignore = 1;
static const long asn_VAL_18_id_InitialULRRCMessageTransfer = 11;
static const long asn_VAL_18_ignore = 1;
static const long asn_VAL_19_id_SystemInformationDeliveryCommand = 17;
static const long asn_VAL_19_ignore = 1;
static const long asn_VAL_20_id_Paging = 18;
static const long asn_VAL_20_ignore = 1;
static const long asn_VAL_21_id_Notify = 19;
static const long asn_VAL_21_ignore = 1;
static const long asn_VAL_22_id_PWSRestartIndication = 22;
static const long asn_VAL_22_ignore = 1;
static const long asn_VAL_23_id_PWSFailureIndication = 23;
static const long asn_VAL_23_ignore = 1;
static const long asn_VAL_24_id_GNBDUStatusIndication = 24;
static const long asn_VAL_24_ignore = 1;
static const long asn_VAL_25_id_RRCDeliveryReport = 25;
static const long asn_VAL_25_ignore = 1;
static const asn_ioc_cell_t asn_IOS_F1AP_ELEMENTARY_PROCEDURES_1_rows[] = {
	{ "&InitiatingMessage", aioc__type, &asn_DEF_Reset },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_ResetAcknowledge },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_1_id_Reset },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_1_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_F1SetupRequest },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_F1SetupResponse },
	{ "&UnsuccessfulOutcome", aioc__type, &asn_DEF_F1SetupFailure },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_2_id_F1Setup },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_2_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_GNBDUConfigurationUpdate },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_GNBDUConfigurationUpdateAcknowledge },
	{ "&UnsuccessfulOutcome", aioc__type, &asn_DEF_GNBDUConfigurationUpdateFailure },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_3_id_gNBDUConfigurationUpdate },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_3_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_GNBCUConfigurationUpdate },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_GNBCUConfigurationUpdateAcknowledge },
	{ "&UnsuccessfulOutcome", aioc__type, &asn_DEF_GNBCUConfigurationUpdateFailure },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_4_id_gNBCUConfigurationUpdate },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_4_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_UEContextSetupRequest },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_UEContextSetupResponse },
	{ "&UnsuccessfulOutcome", aioc__type, &asn_DEF_UEContextSetupFailure },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_5_id_UEContextSetup },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_5_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_UEContextReleaseCommand },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_UEContextReleaseComplete },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_6_id_UEContextRelease },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_6_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_UEContextModificationRequest },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_UEContextModificationResponse },
	{ "&UnsuccessfulOutcome", aioc__type, &asn_DEF_UEContextModificationFailure },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_7_id_UEContextModification },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_7_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_UEContextModificationRequired },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_UEContextModificationConfirm },
	{ "&UnsuccessfulOutcome", aioc__type, &asn_DEF_UEContextModificationRefuse },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_8_id_UEContextModificationRequired },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_8_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_WriteReplaceWarningRequest },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_WriteReplaceWarningResponse },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_9_id_WriteReplaceWarning },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_9_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_PWSCancelRequest },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_PWSCancelResponse },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_10_id_PWSCancel },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_10_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_GNBDUResourceCoordinationRequest },
	{ "&SuccessfulOutcome", aioc__type, &asn_DEF_GNBDUResourceCoordinationResponse },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_11_id_GNBDUResourceCoordination },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_11_reject },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_ErrorIndication },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_12_id_ErrorIndication },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_12_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_UEContextReleaseRequest },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_13_id_UEContextReleaseRequest },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_13_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_DLRRCMessageTransfer },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_14_id_DLRRCMessageTransfer },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_14_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_ULRRCMessageTransfer },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_15_id_ULRRCMessageTransfer },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_15_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_UEInactivityNotification },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_16_id_UEInactivityNotification },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_16_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_PrivateMessage },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_17_id_privateMessage },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_17_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_InitialULRRCMessageTransfer },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_18_id_InitialULRRCMessageTransfer },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_18_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_SystemInformationDeliveryCommand },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_19_id_SystemInformationDeliveryCommand },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_19_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_Paging },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_20_id_Paging },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_20_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_Notify },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_21_id_Notify },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_21_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_PWSRestartIndication },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_22_id_PWSRestartIndication },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_22_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_PWSFailureIndication },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_23_id_PWSFailureIndication },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_23_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_GNBDUStatusIndication },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_24_id_GNBDUStatusIndication },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_24_ignore },
	{ "&InitiatingMessage", aioc__type, &asn_DEF_RRCDeliveryReport },
	{ "&SuccessfulOutcome",  },
	{ "&UnsuccessfulOutcome",  },
	{ "&procedureCode", aioc__value, &asn_DEF_ProcedureCode, &asn_VAL_25_id_RRCDeliveryReport },
	{ "&criticality", aioc__value, &asn_DEF_Criticality, &asn_VAL_25_ignore }
};
static const asn_ioc_set_t asn_IOS_F1AP_ELEMENTARY_PROCEDURES_1[] = {
	{ 25, 5, asn_IOS_F1AP_ELEMENTARY_PROCEDURES_1_rows }
};
static int
memb_procedureCode_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0L && value <= 255L)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_type_selector_result_t
select_SuccessfulOutcome_criticality_type(const asn_TYPE_descriptor_t *parent_type, const void *parent_sptr) {
	asn_type_selector_result_t result = {0, 0};
	const asn_ioc_set_t *itable = asn_IOS_F1AP_ELEMENTARY_PROCEDURES_1;
	size_t constraining_column = 3; /* &procedureCode */
	size_t for_column = 4; /* &criticality */
	size_t row, presence_index = 0;
	const long *constraining_value = (const long *)((const char *)parent_sptr + offsetof(struct SuccessfulOutcome, procedureCode));
	
	for(row=0; row < itable->rows_count; row++) {
	    const asn_ioc_cell_t *constraining_cell = &itable->rows[row * itable->columns_count + constraining_column];
	    const asn_ioc_cell_t *type_cell = &itable->rows[row * itable->columns_count + for_column];
	
	    if(type_cell->cell_kind == aioc__undefined)
	        continue;
	
	    presence_index++;
	    if(constraining_cell->type_descriptor->op->compare_struct(constraining_cell->type_descriptor, constraining_value, constraining_cell->value_sptr) == 0) {
	        result.type_descriptor = type_cell->type_descriptor;
	        result.presence_index = presence_index;
	        break;
	    }
	}
	
	return result;
}

static int
memb_criticality_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	
	if(1 /* No applicable constraints whatsoever */) {
		/* Nothing is here. See below */
	}
	
	return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
}

static asn_type_selector_result_t
select_SuccessfulOutcome_value_type(const asn_TYPE_descriptor_t *parent_type, const void *parent_sptr) {
	asn_type_selector_result_t result = {0, 0};
	const asn_ioc_set_t *itable = asn_IOS_F1AP_ELEMENTARY_PROCEDURES_1;
	size_t constraining_column = 3; /* &procedureCode */
	size_t for_column = 1; /* &SuccessfulOutcome */
	size_t row, presence_index = 0;
	const long *constraining_value = (const long *)((const char *)parent_sptr + offsetof(struct SuccessfulOutcome, procedureCode));
	
	for(row=0; row < itable->rows_count; row++) {
	    const asn_ioc_cell_t *constraining_cell = &itable->rows[row * itable->columns_count + constraining_column];
	    const asn_ioc_cell_t *type_cell = &itable->rows[row * itable->columns_count + for_column];
	
	    if(type_cell->cell_kind == aioc__undefined)
	        continue;
	
	    presence_index++;
	    if(constraining_cell->type_descriptor->op->compare_struct(constraining_cell->type_descriptor, constraining_value, constraining_cell->value_sptr) == 0) {
	        result.type_descriptor = type_cell->type_descriptor;
	        result.presence_index = presence_index;
	        break;
	    }
	}
	
	return result;
}

static int
memb_value_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	
	if(1 /* No applicable constraints whatsoever */) {
		/* Nothing is here. See below */
	}
	
	return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
}

#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_memb_procedureCode_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 8,  8,  0,  255 }	/* (0..255) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_memb_criticality_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_memb_value_constr_4 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
static asn_TYPE_member_t asn_MBR_value_4[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.ResetAcknowledge),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ResetAcknowledge,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"ResetAcknowledge"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.F1SetupResponse),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_F1SetupResponse,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"F1SetupResponse"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.GNBDUConfigurationUpdateAcknowledge),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_GNBDUConfigurationUpdateAcknowledge,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"GNBDUConfigurationUpdateAcknowledge"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.GNBCUConfigurationUpdateAcknowledge),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_GNBCUConfigurationUpdateAcknowledge,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"GNBCUConfigurationUpdateAcknowledge"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.UEContextSetupResponse),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_UEContextSetupResponse,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"UEContextSetupResponse"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.UEContextReleaseComplete),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_UEContextReleaseComplete,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"UEContextReleaseComplete"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.UEContextModificationResponse),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_UEContextModificationResponse,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"UEContextModificationResponse"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.UEContextModificationConfirm),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_UEContextModificationConfirm,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"UEContextModificationConfirm"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.WriteReplaceWarningResponse),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_WriteReplaceWarningResponse,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"WriteReplaceWarningResponse"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.PWSCancelResponse),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_PWSCancelResponse,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"PWSCancelResponse"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome__value, choice.GNBDUResourceCoordinationResponse),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_GNBDUResourceCoordinationResponse,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"GNBDUResourceCoordinationResponse"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_value_tag2el_4[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 0, 0, 10 }, /* ResetAcknowledge */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 1, -1, 9 }, /* F1SetupResponse */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 2, -2, 8 }, /* GNBDUConfigurationUpdateAcknowledge */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 3, -3, 7 }, /* GNBCUConfigurationUpdateAcknowledge */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 4, -4, 6 }, /* UEContextSetupResponse */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 5, -5, 5 }, /* UEContextReleaseComplete */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 6, -6, 4 }, /* UEContextModificationResponse */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 7, -7, 3 }, /* UEContextModificationConfirm */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 8, -8, 2 }, /* WriteReplaceWarningResponse */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 9, -9, 1 }, /* PWSCancelResponse */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 10, -10, 0 } /* GNBDUResourceCoordinationResponse */
};
static asn_CHOICE_specifics_t asn_SPC_value_specs_4 = {
	sizeof(struct SuccessfulOutcome__value),
	offsetof(struct SuccessfulOutcome__value, _asn_ctx),
	offsetof(struct SuccessfulOutcome__value, present),
	sizeof(((struct SuccessfulOutcome__value *)0)->present),
	asn_MAP_value_tag2el_4,
	11,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_value_4 = {
	"value",
	"value",
	&asn_OP_OPEN_TYPE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		OPEN_TYPE_constraint
	},
	asn_MBR_value_4,
	11,	/* Elements count */
	&asn_SPC_value_specs_4	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SuccessfulOutcome_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome, procedureCode),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProcedureCode,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			&asn_PER_memb_procedureCode_constr_2,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			memb_procedureCode_constraint_1
		},
		0, 0, /* No default value */
		"procedureCode"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome, criticality),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Criticality,
		select_SuccessfulOutcome_criticality_type,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			&asn_PER_memb_criticality_constr_3,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			memb_criticality_constraint_1
		},
		0, 0, /* No default value */
		"criticality"
		},
	{ ATF_OPEN_TYPE | ATF_NOFLAGS, 0, offsetof(struct SuccessfulOutcome, value),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_value_4,
		select_SuccessfulOutcome_value_type,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			&asn_PER_memb_value_constr_4,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			memb_value_constraint_1
		},
		0, 0, /* No default value */
		"value"
		},
};
static const ber_tlv_tag_t asn_DEF_SuccessfulOutcome_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SuccessfulOutcome_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* procedureCode */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* criticality */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* value */
};
asn_SEQUENCE_specifics_t asn_SPC_SuccessfulOutcome_specs_1 = {
	sizeof(struct SuccessfulOutcome),
	offsetof(struct SuccessfulOutcome, _asn_ctx),
	asn_MAP_SuccessfulOutcome_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SuccessfulOutcome = {
	"SuccessfulOutcome",
	"SuccessfulOutcome",
	&asn_OP_SEQUENCE,
	asn_DEF_SuccessfulOutcome_tags_1,
	sizeof(asn_DEF_SuccessfulOutcome_tags_1)
		/sizeof(asn_DEF_SuccessfulOutcome_tags_1[0]), /* 1 */
	asn_DEF_SuccessfulOutcome_tags_1,	/* Same as above */
	sizeof(asn_DEF_SuccessfulOutcome_tags_1)
		/sizeof(asn_DEF_SuccessfulOutcome_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		SEQUENCE_constraint
	},
	asn_MBR_SuccessfulOutcome_1,
	3,	/* Elements count */
	&asn_SPC_SuccessfulOutcome_specs_1	/* Additional specs */
};

