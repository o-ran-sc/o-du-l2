/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-Containers"
 * 	found in "/root/svaidhya/ASN1_file/E2APV0300.asn1"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2AP/`
 */

#ifndef	_ProtocolIE_ContainerE2_H_
#define	_ProtocolIE_ContainerE2_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct RICsubscriptionRequest_IEs;
struct RICsubscriptionResponse_IEs;
struct RICsubscriptionFailure_IEs;
struct RICsubscriptionDeleteRequest_IEs;
struct RICsubscriptionDeleteResponse_IEs;
struct RICsubscriptionDeleteFailure_IEs;
struct RICsubscriptionDeleteRequired_IEs;
struct RICsubscriptionModificationRequest_IEs;
struct RICsubscriptionModificationResponse_IEs;
struct RICsubscriptionModificationFailure_IEs;
struct RICsubscriptionModificationRequired_IEs;
struct RICsubscriptionModificationConfirm_IEs;
struct RICsubscriptionModificationRefuse_IEs;
struct RICindication_IEs;
struct RICcontrolRequest_IEs;
struct RICcontrolAcknowledge_IEs;
struct RICcontrolFailure_IEs;
struct RICQueryRequest_IEs;
struct RICQueryResponse_IEs;
struct RICQueryFailure_IEs;
struct ErrorIndicationE2_IEs;
struct E2setupRequestIEs;
struct E2setupResponseIEs;
struct E2setupFailureIEs;
struct E2connectionUpdate_IEs;
struct E2connectionUpdateAck_IEs;
struct E2connectionUpdateFailure_IEs;
struct E2nodeConfigurationUpdate_IEs;
struct E2nodeConfigurationUpdateAcknowledge_IEs;
struct E2nodeConfigurationUpdateFailure_IEs;
struct ResetRequestIEs;
struct ResetResponseIEs;
struct RICserviceUpdate_IEs;
struct RICserviceUpdateAcknowledge_IEs;
struct RICserviceUpdateFailure_IEs;
struct RICserviceQuery_IEs;
struct E2RemovalRequestIEs;
struct E2RemovalResponseIEs;
struct E2RemovalFailureIEs;

/* ProtocolIE-ContainerE2 */
typedef struct ProtocolIE_ContainerE2_2530P0 {
	A_SEQUENCE_OF(struct RICsubscriptionRequest_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P0_t;
typedef struct ProtocolIE_ContainerE2_2530P1 {
	A_SEQUENCE_OF(struct RICsubscriptionResponse_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P1_t;
typedef struct ProtocolIE_ContainerE2_2530P2 {
	A_SEQUENCE_OF(struct RICsubscriptionFailure_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P2_t;
typedef struct ProtocolIE_ContainerE2_2530P3 {
	A_SEQUENCE_OF(struct RICsubscriptionDeleteRequest_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P3_t;
typedef struct ProtocolIE_ContainerE2_2530P4 {
	A_SEQUENCE_OF(struct RICsubscriptionDeleteResponse_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P4_t;
typedef struct ProtocolIE_ContainerE2_2530P5 {
	A_SEQUENCE_OF(struct RICsubscriptionDeleteFailure_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P5_t;
typedef struct ProtocolIE_ContainerE2_2530P6 {
	A_SEQUENCE_OF(struct RICsubscriptionDeleteRequired_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P6_t;
typedef struct ProtocolIE_ContainerE2_2530P7 {
	A_SEQUENCE_OF(struct RICsubscriptionModificationRequest_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P7_t;
typedef struct ProtocolIE_ContainerE2_2530P8 {
	A_SEQUENCE_OF(struct RICsubscriptionModificationResponse_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P8_t;
typedef struct ProtocolIE_ContainerE2_2530P9 {
	A_SEQUENCE_OF(struct RICsubscriptionModificationFailure_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P9_t;
typedef struct ProtocolIE_ContainerE2_2530P10 {
	A_SEQUENCE_OF(struct RICsubscriptionModificationRequired_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P10_t;
typedef struct ProtocolIE_ContainerE2_2530P11 {
	A_SEQUENCE_OF(struct RICsubscriptionModificationConfirm_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P11_t;
typedef struct ProtocolIE_ContainerE2_2530P12 {
	A_SEQUENCE_OF(struct RICsubscriptionModificationRefuse_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P12_t;
typedef struct ProtocolIE_ContainerE2_2530P13 {
	A_SEQUENCE_OF(struct RICindication_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P13_t;
typedef struct ProtocolIE_ContainerE2_2530P14 {
	A_SEQUENCE_OF(struct RICcontrolRequest_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P14_t;
typedef struct ProtocolIE_ContainerE2_2530P15 {
	A_SEQUENCE_OF(struct RICcontrolAcknowledge_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P15_t;
typedef struct ProtocolIE_ContainerE2_2530P16 {
	A_SEQUENCE_OF(struct RICcontrolFailure_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P16_t;
typedef struct ProtocolIE_ContainerE2_2530P17 {
	A_SEQUENCE_OF(struct RICQueryRequest_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P17_t;
typedef struct ProtocolIE_ContainerE2_2530P18 {
	A_SEQUENCE_OF(struct RICQueryResponse_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P18_t;
typedef struct ProtocolIE_ContainerE2_2530P19 {
	A_SEQUENCE_OF(struct RICQueryFailure_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P19_t;
typedef struct ProtocolIE_ContainerE2_2530P20 {
	A_SEQUENCE_OF(struct ErrorIndicationE2_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P20_t;
typedef struct ProtocolIE_ContainerE2_2530P21 {
	A_SEQUENCE_OF(struct E2setupRequestIEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P21_t;
typedef struct ProtocolIE_ContainerE2_2530P22 {
	A_SEQUENCE_OF(struct E2setupResponseIEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P22_t;
typedef struct ProtocolIE_ContainerE2_2530P23 {
	A_SEQUENCE_OF(struct E2setupFailureIEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P23_t;
typedef struct ProtocolIE_ContainerE2_2530P24 {
	A_SEQUENCE_OF(struct E2connectionUpdate_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P24_t;
typedef struct ProtocolIE_ContainerE2_2530P25 {
	A_SEQUENCE_OF(struct E2connectionUpdateAck_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P25_t;
typedef struct ProtocolIE_ContainerE2_2530P26 {
	A_SEQUENCE_OF(struct E2connectionUpdateFailure_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P26_t;
typedef struct ProtocolIE_ContainerE2_2530P27 {
	A_SEQUENCE_OF(struct E2nodeConfigurationUpdate_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P27_t;
typedef struct ProtocolIE_ContainerE2_2530P28 {
	A_SEQUENCE_OF(struct E2nodeConfigurationUpdateAcknowledge_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P28_t;
typedef struct ProtocolIE_ContainerE2_2530P29 {
	A_SEQUENCE_OF(struct E2nodeConfigurationUpdateFailure_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P29_t;
typedef struct ProtocolIE_ContainerE2_2530P30 {
	A_SEQUENCE_OF(struct ResetRequestIEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P30_t;
typedef struct ProtocolIE_ContainerE2_2530P31 {
	A_SEQUENCE_OF(struct ResetResponseIEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P31_t;
typedef struct ProtocolIE_ContainerE2_2530P32 {
	A_SEQUENCE_OF(struct RICserviceUpdate_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P32_t;
typedef struct ProtocolIE_ContainerE2_2530P33 {
	A_SEQUENCE_OF(struct RICserviceUpdateAcknowledge_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P33_t;
typedef struct ProtocolIE_ContainerE2_2530P34 {
	A_SEQUENCE_OF(struct RICserviceUpdateFailure_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P34_t;
typedef struct ProtocolIE_ContainerE2_2530P35 {
	A_SEQUENCE_OF(struct RICserviceQuery_IEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P35_t;
typedef struct ProtocolIE_ContainerE2_2530P36 {
	A_SEQUENCE_OF(struct E2RemovalRequestIEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P36_t;
typedef struct ProtocolIE_ContainerE2_2530P37 {
	A_SEQUENCE_OF(struct E2RemovalResponseIEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P37_t;
typedef struct ProtocolIE_ContainerE2_2530P38 {
	A_SEQUENCE_OF(struct E2RemovalFailureIEs) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolIE_ContainerE2_2530P38_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P0;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P0_specs_1;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P0_1[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P0_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P1;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P1_specs_3;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P1_3[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P1_constr_3;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P2;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P2_specs_5;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P2_5[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P2_constr_5;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P3;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P3_specs_7;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P3_7[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P3_constr_7;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P4;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P4_specs_9;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P4_9[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P4_constr_9;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P5;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P5_specs_11;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P5_11[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P5_constr_11;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P6;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P6_specs_13;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P6_13[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P6_constr_13;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P7;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P7_specs_15;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P7_15[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P7_constr_15;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P8;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P8_specs_17;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P8_17[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P8_constr_17;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P9;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P9_specs_19;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P9_19[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P9_constr_19;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P10;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P10_specs_21;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P10_21[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P10_constr_21;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P11;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P11_specs_23;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P11_23[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P11_constr_23;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P12;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P12_specs_25;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P12_25[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P12_constr_25;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P13;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P13_specs_27;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P13_27[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P13_constr_27;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P14;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P14_specs_29;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P14_29[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P14_constr_29;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P15;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P15_specs_31;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P15_31[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P15_constr_31;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P16;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P16_specs_33;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P16_33[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P16_constr_33;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P17;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P17_specs_35;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P17_35[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P17_constr_35;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P18;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P18_specs_37;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P18_37[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P18_constr_37;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P19;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P19_specs_39;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P19_39[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P19_constr_39;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P20;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P20_specs_41;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P20_41[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P20_constr_41;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P21;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P21_specs_43;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P21_43[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P21_constr_43;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P22;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P22_specs_45;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P22_45[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P22_constr_45;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P23;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P23_specs_47;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P23_47[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P23_constr_47;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P24;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P24_specs_49;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P24_49[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P24_constr_49;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P25;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P25_specs_51;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P25_51[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P25_constr_51;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P26;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P26_specs_53;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P26_53[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P26_constr_53;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P27;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P27_specs_55;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P27_55[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P27_constr_55;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P28;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P28_specs_57;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P28_57[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P28_constr_57;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P29;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P29_specs_59;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P29_59[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P29_constr_59;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P30;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P30_specs_61;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P30_61[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P30_constr_61;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P31;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P31_specs_63;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P31_63[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P31_constr_63;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P32;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P32_specs_65;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P32_65[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P32_constr_65;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P33;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P33_specs_67;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P33_67[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P33_constr_67;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P34;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P34_specs_69;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P34_69[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P34_constr_69;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P35;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P35_specs_71;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P35_71[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P35_constr_71;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P36;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P36_specs_73;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P36_73[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P36_constr_73;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P37;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P37_specs_75;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P37_75[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P37_constr_75;
extern asn_TYPE_descriptor_t asn_DEF_ProtocolIE_ContainerE2_2530P38;
extern asn_SET_OF_specifics_t asn_SPC_ProtocolIE_ContainerE2_2530P38_specs_77;
extern asn_TYPE_member_t asn_MBR_ProtocolIE_ContainerE2_2530P38_77[1];
extern asn_per_constraints_t asn_PER_type_ProtocolIE_ContainerE2_2530P38_constr_77;

#ifdef __cplusplus
}
#endif

#endif	/* _ProtocolIE_ContainerE2_H_ */
#include <asn_internal.h>
