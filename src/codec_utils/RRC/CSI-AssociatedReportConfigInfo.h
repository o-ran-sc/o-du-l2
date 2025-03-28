/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_CSI_AssociatedReportConfigInfo_H_
#define	_CSI_AssociatedReportConfigInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CSI-ReportConfigId.h"
#include <NativeInteger.h>
#include "TCI-StateId.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CSI_AssociatedReportConfigInfo__resourcesForChannel_PR {
	CSI_AssociatedReportConfigInfo__resourcesForChannel_PR_NOTHING,	/* No components present */
	CSI_AssociatedReportConfigInfo__resourcesForChannel_PR_nzp_CSI_RS,
	CSI_AssociatedReportConfigInfo__resourcesForChannel_PR_csi_SSB_ResourceSet
} CSI_AssociatedReportConfigInfo__resourcesForChannel_PR;

/* CSI-AssociatedReportConfigInfo */
typedef struct CSI_AssociatedReportConfigInfo {
	CSI_ReportConfigId_t	 reportConfigId;
	struct CSI_AssociatedReportConfigInfo__resourcesForChannel {
		CSI_AssociatedReportConfigInfo__resourcesForChannel_PR present;
		union CSI_AssociatedReportConfigInfo__resourcesForChannel_u {
			struct CSI_AssociatedReportConfigInfo__resourcesForChannel__nzp_CSI_RS {
				long	 resourceSet;
				struct CSI_AssociatedReportConfigInfo__resourcesForChannel__nzp_CSI_RS__qcl_info {
					A_SEQUENCE_OF(TCI_StateId_t) list;
					
					/* Context for parsing across buffer boundaries */
					asn_struct_ctx_t _asn_ctx;
				} *qcl_info;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *nzp_CSI_RS;
			long	 csi_SSB_ResourceSet;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} resourcesForChannel;
	long	*csi_IM_ResourcesForInterference;	/* OPTIONAL */
	long	*nzp_CSI_RS_ResourcesForInterference;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CSI_AssociatedReportConfigInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CSI_AssociatedReportConfigInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_CSI_AssociatedReportConfigInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_CSI_AssociatedReportConfigInfo_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _CSI_AssociatedReportConfigInfo_H_ */
#include <asn_internal.h>
