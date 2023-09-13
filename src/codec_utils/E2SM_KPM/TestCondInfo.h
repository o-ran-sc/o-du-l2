/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "../../ASN1_Input/E2SM_KPM_V_3_0.asn1"
 * 	`asn1c -D ./../../E2_output/E2SM_KPM_v3.0_output -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_TestCondInfo_H_
#define	_TestCondInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TestCond-Type.h"
#include "TestCond-Expression.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct TestCond_Value;

/* TestCondInfo */
typedef struct TestCondInfo {
	TestCond_Type_t	 testType;
	TestCond_Expression_t	*testExpr;	/* OPTIONAL */
	struct TestCond_Value	*testValue;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TestCondInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TestCondInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_TestCondInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_TestCondInfo_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _TestCondInfo_H_ */
#include <asn_internal.h>