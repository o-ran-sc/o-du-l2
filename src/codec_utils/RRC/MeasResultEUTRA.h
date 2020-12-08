/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasResultEUTRA_H_
#define	_MeasResultEUTRA_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PhysCellId.h"
#include "MeasQuantityResultsEUTRA.h"
#include "FreqBandIndicatorEUTRA.h"
#include <NativeEnumerated.h>
#include "CellAccessRelatedInfo-EUTRA-EPC.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MeasResultEUTRA__cgi_Info__freqBandIndicatorPriority {
	MeasResultEUTRA__cgi_Info__freqBandIndicatorPriority_true	= 0
} e_MeasResultEUTRA__cgi_Info__freqBandIndicatorPriority;

/* Forward declarations */
struct MultiBandInfoListEUTRA;
struct CellAccessRelatedInfo_EUTRA_EPC;
struct CellAccessRelatedInfo_EUTRA_5GC;

/* MeasResultEUTRA */
typedef struct MeasResultEUTRA {
	PhysCellId_t	 eutra_PhysCellId;
	MeasQuantityResultsEUTRA_t	 measResult;
	struct MeasResultEUTRA__cgi_Info {
		struct MeasResultEUTRA__cgi_Info__cgi_info_EPC {
			CellAccessRelatedInfo_EUTRA_EPC_t	 cgi_info_EPC_legacy;
			struct MeasResultEUTRA__cgi_Info__cgi_info_EPC__cgi_info_EPC_list {
				A_SEQUENCE_OF(struct CellAccessRelatedInfo_EUTRA_EPC) list;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *cgi_info_EPC_list;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *cgi_info_EPC;
		struct MeasResultEUTRA__cgi_Info__cgi_info_5GC {
			A_SEQUENCE_OF(struct CellAccessRelatedInfo_EUTRA_5GC) list;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *cgi_info_5GC;
		FreqBandIndicatorEUTRA_t	 freqBandIndicator;
		struct MultiBandInfoListEUTRA	*multiBandInfoList;	/* OPTIONAL */
		long	*freqBandIndicatorPriority;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *cgi_Info;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasResultEUTRA_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_freqBandIndicatorPriority_13;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_MeasResultEUTRA;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasResultEUTRA_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasResultEUTRA_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _MeasResultEUTRA_H_ */
#include <asn_internal.h>
