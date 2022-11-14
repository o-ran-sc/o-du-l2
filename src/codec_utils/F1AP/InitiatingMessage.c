/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Descriptions"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "InitiatingMessage.h"

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
	
	if((value >= 0 && value <= 255)) {
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
select_InitiatingMessage_criticality_type(const asn_TYPE_descriptor_t *parent_type, const void *parent_sptr) {
	asn_type_selector_result_t result = {0, 0};
	const asn_ioc_set_t *itable = asn_IOS_F1AP_ELEMENTARY_PROCEDURES_1;
	size_t constraining_column = 3; /* &procedureCode */
	size_t for_column = 4; /* &criticality */
	size_t row, presence_index = 0;
	const long *constraining_value = (const long *)((const char *)parent_sptr + offsetof(struct InitiatingMessage, procedureCode));
	
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
select_InitiatingMessage_value_type(const asn_TYPE_descriptor_t *parent_type, const void *parent_sptr) {
	asn_type_selector_result_t result = {0, 0};
	const asn_ioc_set_t *itable = asn_IOS_F1AP_ELEMENTARY_PROCEDURES_1;
	size_t constraining_column = 3; /* &procedureCode */
	size_t for_column = 0; /* &InitiatingMessage */
	size_t row, presence_index = 0;
	const long *constraining_value = (const long *)((const char *)parent_sptr + offsetof(struct InitiatingMessage, procedureCode));
	
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

static asn_oer_constraints_t asn_OER_memb_procedureCode_constr_2 CC_NOTUSED = {
	{ 1, 1 }	/* (0..255) */,
	-1};
static asn_per_constraints_t asn_PER_memb_procedureCode_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 8,  8,  0,  255 }	/* (0..255) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_criticality_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_memb_criticality_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_value_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_memb_value_constr_4 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_value_4[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.Reset),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_Reset,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"Reset"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.F1SetupRequest),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_F1SetupRequest,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"F1SetupRequest"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.GNBDUConfigurationUpdate),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_GNBDUConfigurationUpdate,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"GNBDUConfigurationUpdate"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.GNBCUConfigurationUpdate),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_GNBCUConfigurationUpdate,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"GNBCUConfigurationUpdate"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.UEContextSetupRequest),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_UEContextSetupRequest,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"UEContextSetupRequest"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.UEContextReleaseCommand),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_UEContextReleaseCommand,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"UEContextReleaseCommand"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.UEContextModificationRequest),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_UEContextModificationRequest,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"UEContextModificationRequest"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.UEContextModificationRequired),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_UEContextModificationRequired,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"UEContextModificationRequired"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.WriteReplaceWarningRequest),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_WriteReplaceWarningRequest,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"WriteReplaceWarningRequest"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.PWSCancelRequest),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_PWSCancelRequest,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"PWSCancelRequest"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.GNBDUResourceCoordinationRequest),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_GNBDUResourceCoordinationRequest,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"GNBDUResourceCoordinationRequest"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.ErrorIndication),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ErrorIndication,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ErrorIndication"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.UEContextReleaseRequest),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_UEContextReleaseRequest,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"UEContextReleaseRequest"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.DLRRCMessageTransfer),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_DLRRCMessageTransfer,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"DLRRCMessageTransfer"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.ULRRCMessageTransfer),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ULRRCMessageTransfer,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ULRRCMessageTransfer"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.UEInactivityNotification),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_UEInactivityNotification,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"UEInactivityNotification"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.PrivateMessage),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_PrivateMessage,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"PrivateMessage"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.InitialULRRCMessageTransfer),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_InitialULRRCMessageTransfer,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"InitialULRRCMessageTransfer"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.SystemInformationDeliveryCommand),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_SystemInformationDeliveryCommand,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"SystemInformationDeliveryCommand"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.Paging),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_Paging,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"Paging"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.Notify),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_Notify,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"Notify"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.PWSRestartIndication),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_PWSRestartIndication,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"PWSRestartIndication"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.PWSFailureIndication),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_PWSFailureIndication,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"PWSFailureIndication"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.GNBDUStatusIndication),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_GNBDUStatusIndication,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"GNBDUStatusIndication"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage__value, choice.RRCDeliveryReport),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_RRCDeliveryReport,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"RRCDeliveryReport"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_value_tag2el_4[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 0, 0, 24 }, /* Reset */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 1, -1, 23 }, /* F1SetupRequest */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 2, -2, 22 }, /* GNBDUConfigurationUpdate */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 3, -3, 21 }, /* GNBCUConfigurationUpdate */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 4, -4, 20 }, /* UEContextSetupRequest */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 5, -5, 19 }, /* UEContextReleaseCommand */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 6, -6, 18 }, /* UEContextModificationRequest */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 7, -7, 17 }, /* UEContextModificationRequired */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 8, -8, 16 }, /* WriteReplaceWarningRequest */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 9, -9, 15 }, /* PWSCancelRequest */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 10, -10, 14 }, /* GNBDUResourceCoordinationRequest */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 11, -11, 13 }, /* ErrorIndication */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 12, -12, 12 }, /* UEContextReleaseRequest */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 13, -13, 11 }, /* DLRRCMessageTransfer */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 14, -14, 10 }, /* ULRRCMessageTransfer */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 15, -15, 9 }, /* UEInactivityNotification */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 16, -16, 8 }, /* PrivateMessage */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 17, -17, 7 }, /* InitialULRRCMessageTransfer */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 18, -18, 6 }, /* SystemInformationDeliveryCommand */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 19, -19, 5 }, /* Paging */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 20, -20, 4 }, /* Notify */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 21, -21, 3 }, /* PWSRestartIndication */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 22, -22, 2 }, /* PWSFailureIndication */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 23, -23, 1 }, /* GNBDUStatusIndication */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 24, -24, 0 } /* RRCDeliveryReport */
};
static asn_CHOICE_specifics_t asn_SPC_value_specs_4 = {
	sizeof(struct InitiatingMessage__value),
	offsetof(struct InitiatingMessage__value, _asn_ctx),
	offsetof(struct InitiatingMessage__value, present),
	sizeof(((struct InitiatingMessage__value *)0)->present),
	asn_MAP_value_tag2el_4,
	25,	/* Count of tags in the map */
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
	{ 0, 0, OPEN_TYPE_constraint },
	asn_MBR_value_4,
	25,	/* Elements count */
	&asn_SPC_value_specs_4	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_InitiatingMessage_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage, procedureCode),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProcedureCode,
		0,
		{ &asn_OER_memb_procedureCode_constr_2, &asn_PER_memb_procedureCode_constr_2,  memb_procedureCode_constraint_1 },
		0, 0, /* No default value */
		"procedureCode"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage, criticality),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Criticality,
		select_InitiatingMessage_criticality_type,
		{ &asn_OER_memb_criticality_constr_3, &asn_PER_memb_criticality_constr_3,  memb_criticality_constraint_1 },
		0, 0, /* No default value */
		"criticality"
		},
	{ ATF_OPEN_TYPE | ATF_NOFLAGS, 0, offsetof(struct InitiatingMessage, value),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_value_4,
		select_InitiatingMessage_value_type,
		{ &asn_OER_memb_value_constr_4, &asn_PER_memb_value_constr_4,  memb_value_constraint_1 },
		0, 0, /* No default value */
		"value"
		},
};
static const ber_tlv_tag_t asn_DEF_InitiatingMessage_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_InitiatingMessage_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* procedureCode */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* criticality */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* value */
};
asn_SEQUENCE_specifics_t asn_SPC_InitiatingMessage_specs_1 = {
	sizeof(struct InitiatingMessage),
	offsetof(struct InitiatingMessage, _asn_ctx),
	asn_MAP_InitiatingMessage_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_InitiatingMessage = {
	"InitiatingMessage",
	"InitiatingMessage",
	&asn_OP_SEQUENCE,
	asn_DEF_InitiatingMessage_tags_1,
	sizeof(asn_DEF_InitiatingMessage_tags_1)
		/sizeof(asn_DEF_InitiatingMessage_tags_1[0]), /* 1 */
	asn_DEF_InitiatingMessage_tags_1,	/* Same as above */
	sizeof(asn_DEF_InitiatingMessage_tags_1)
		/sizeof(asn_DEF_InitiatingMessage_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_InitiatingMessage_1,
	3,	/* Elements count */
	&asn_SPC_InitiatingMessage_specs_1	/* Additional specs */
};

