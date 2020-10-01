################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
#
##-------------------------------------------------------------#
#Makefile for product mt - script generated.
#Only the $(CCmtFLAGS) may be modified.
#-------------------------------------------------------------#
include ../common/rsys_fancy.mak
include ../common/env.mak

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCmtFLAGS=-DSS_MT_TMR -USS_LOGGER_SUPPORT -DEGTP_U#<---Insert mt specific defines here

#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#
SRC_DIR=$(ROOT_DIR)/src/mt

C_SRCS=$(wildcard $(SRC_DIR)/*.c)
C_SRCS:=$(filter-out %ss_acc.c,$(C_SRCS))
C_SRCS:=$(filter-out %mt_tst.c,$(C_SRCS))
C_SRCS:=$(filter-out %sol.c, $(C_SRCS))
C_SRCS:=$(filter-out %diag.c, $(C_SRCS))
C_SRCS:=$(filter-out %mtsp.c, $(C_SRCS))
C_SRCS:=$(filter-out %tskent.c, $(C_SRCS))

ifeq ($(PLTFRM),PAL)
C_SRCS:=$(filter-out %4gmx.c, $(C_SRCS))
endif
ifneq ($(BOARD), T33H)
C_SRCS:=$(filter-out %t33.c, $(C_SRCS))
endif

C_OBJS_WO_LOG=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))
I_OPTS+=-isystem $(BSPPATH)/usr/include/brcm
I_OPTS+=-I$(SRC_DIR)/security
I-OPTS+=-I$(ROOT_DIR)/src/wls_lib


#mtSRCS= \
#	$(IN_DIR)/mt_id.c\
#	$(IN_DIR)/mt_ss.c\
#	$(IN_DIR)/ss_gen.c\
#	$(IN_DIR)/ss_id.c\
#	$(IN_DIR)/ss_mem.c\
#	$(IN_DIR)/ss_msg.c\
#	$(IN_DIR)/ss_pack.c\
#	$(IN_DIR)/ss_queue.c\
#	$(IN_DIR)/ss_strm.c\
#	$(IN_DIR)/ss_task.c\
#	$(IN_DIR)/ss_drvr.c\
#	$(IN_DIR)/ss_timer.c\
#	$(IN_DIR)/ss_diag.c
#ifeq (${BLDENV},lnx_e2e_ms)
#	mtSRCS+=$(IN_DIR)/mt_4gmx.c
#else
#ifeq (${BLDENV},lnx_ms_withoutdiag)
#	mtSRCS+=$(IN_DIR)/mt_4gmx.c
#endif
#endif
#ifeq ($(BLDENV),lnx_e2e_bc)
#	mtSRCS+=$(IN_DIR)/bc_cpu_overload.c
#	mtSRCS+=$(IN_DIR)/bc_icpu_snd.c
#	mtSRCS+=$(IN_DIR)/bc_icpu_rcv.c
#	mtSRCS+=$(IN_DIR)/bc_cpu_init.c
#	mtSRCS+=$(IN_DIR)/rb_log.c
#	mtSRCS+=$(IN_DIR)/ss_rbuf.c
#	mtSRCS+=$(IN_DIR)/bc_free_mgr.c
#	mtSRCS+=$(IN_DIR)/bc_batch_mgr.c
#else
#	mtSRCS+=$(IN_DIR)/bc_cpu_overload.c
#	mtSRCS+=$(IN_DIR)/bc_icpu_snd.c
#	mtSRCS+=$(IN_DIR)/bc_icpu_rcv.c
#	mtSRCS+=$(IN_DIR)/bc_cpu_init.c
#	mtSRCS+=$(IN_DIR)/rb_log.c
#	mtSRCS+=$(IN_DIR)/ss_rbuf.c
#	mtSRCS+=$(IN_DIR)/bc_free_mgr.c
#	mtSRCS+=$(IN_DIR)/bc_batch_mgr.c
#endif
#
#mtOBJS= \
#	$(OUT_DIR)/mt_id.$(OBJ)\
#	$(OUT_DIR)/mt_ss.$(OBJ)\
#	$(OUT_DIR)/ss_gen.$(OBJ)\
#	$(OUT_DIR)/ss_id.$(OBJ)\
#	$(OUT_DIR)/ss_mem.$(OBJ)\
#	$(OUT_DIR)/ss_msg.$(OBJ)\
#	$(OUT_DIR)/ss_pack.$(OBJ)\
#	$(OUT_DIR)/ss_queue.$(OBJ)\
#	$(OUT_DIR)/ss_strm.$(OBJ)\
#	$(OUT_DIR)/ss_task.$(OBJ)\
#	$(OUT_DIR)/ss_drvr.$(OBJ)\
#	$(OUT_DIR)/ss_timer.$(OBJ)\
#	$(OUT_DIR)/ss_diag.$(OBJ)
#ifeq (${BLDENV},lnx_e2e_ms)
#	mtOBJS+=$(OUT_DIR)/mt_4gmx.$(OBJ)
#else
#ifeq (${BLDENV},lnx_ms_withoutdiag)
#	mtOBJS+=$(OUT_DIR)/mt_4gmx.$(OBJ)
#endif
#endif
#ifeq ($(BLDENV),lnx_e2e_bc)
#	mtOBJS+=$(OUT_DIR)/bc_cpu_overload.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/bc_icpu_snd.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/bc_icpu_rcv.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/bc_cpu_init.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/rb_log.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/ss_rbuf.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/bc_free_mgr.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/bc_batch_mgr.$(OBJ)
#else
#	mtOBJS+=$(OUT_DIR)/bc_cpu_overload.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/bc_icpu_snd.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/bc_icpu_rcv.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/bc_cpu_init.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/rb_log.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/ss_rbuf.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/bc_free_mgr.$(OBJ)
#	mtOBJS+=$(OUT_DIR)/bc_batch_mgr.$(OBJ)
#endif

HDR_FILES+=$(wildcard $(CM_DIR)/env*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/gen*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/ssi*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/cm*.[hx])
HDR_FILES+=$(wildcard $(SRC_DIR)/*.[hx])
HDR_FILES+=$(wildcard $(SRC_DIR)/*.[hx])

#ifeq ($(BLDENV),lnx_e2e_bc)
#	MT_INC+=$(IN_DIR)/bc_icpu.h
#	MT_INC+=$(IN_DIR)/bc_cpu_init.h
#        MT_INC+=$(IN_DIR)/rb_log.x 
#        MT_INC+=$(IN_DIR)/rb_log.h
#        MT_INC+=$(IN_DIR)/bc_cpu_overload.h
#else
#	MT_INC+=$(IN_DIR)/bc_icpu.h
#	MT_INC+=$(IN_DIR)/bc_cpu_init.h
#        MT_INC+=$(IN_DIR)/rb_log.x 
#        MT_INC+=$(IN_DIR)/rb_log.h
#        MT_INC+=$(IN_DIR)/bc_cpu_overload.h
#endif

#ALL_INC=$(MT_INC) $(CM_INC)

#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#$(OUT_DIR)/mt_id.$(OBJ): $(IN_DIR)/mt_id.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/mt_id.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/mt_id.c
#$(OUT_DIR)/mt_ss.$(OBJ): $(IN_DIR)/mt_ss.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/mt_ss.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/mt_ss.c
#ifeq (${BLDENV},lnx_e2e_ms)
#$(OUT_DIR)/mt_4gmx.$(OBJ): $(IN_DIR)/mt_4gmx.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/mt_4gmx.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/mt_4gmx.c
#else
#ifeq (${BLDENV},lnx_ms_withoutdiag)
#$(OUT_DIR)/mt_4gmx.$(OBJ): $(IN_DIR)/mt_4gmx.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/mt_4gmx.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/mt_4gmx.c
#endif
#endif
#
#ifeq ($(BLDENV),lnx_e2e_bc)
#$(OUT_DIR)/bc_cpu_overload.$(OBJ): $(IN_DIR)/bc_cpu_overload.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_cpu_overload.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_cpu_overload.c
#$(OUT_DIR)/bc_icpu_snd.$(OBJ): $(IN_DIR)/bc_icpu_snd.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_icpu_snd.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_icpu_snd.c
#$(OUT_DIR)/bc_icpu_rcv.$(OBJ): $(IN_DIR)/bc_icpu_rcv.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_icpu_rcv.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_icpu_rcv.c
#$(OUT_DIR)/bc_cpu_init.$(OBJ): $(IN_DIR)/bc_cpu_init.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_cpu_init.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_cpu_init.c
#$(OUT_DIR)/rb_log.$(OBJ): $(IN_DIR)/rb_log.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/rb_log.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/rb_log.c
#$(OUT_DIR)/ss_rbuf.$(OBJ): $(IN_DIR)/ss_rbuf.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_rbuf.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_rbuf.c
#$(OUT_DIR)/bc_free_mgr.$(OBJ): $(IN_DIR)/bc_free_mgr.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_free_mgr.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_free_mgr.c
#$(OUT_DIR)/bc_batch_mgr.$(OBJ): $(IN_DIR)/bc_batch_mgr.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_batch_mgr.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_batch_mgr.c
#else
#$(OUT_DIR)/bc_cpu_overload.$(OBJ): $(IN_DIR)/bc_cpu_overload.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_cpu_overload.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_cpu_overload.c
#$(OUT_DIR)/bc_icpu_snd.$(OBJ): $(IN_DIR)/bc_icpu_snd.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_icpu_snd.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_icpu_snd.c
#$(OUT_DIR)/bc_icpu_rcv.$(OBJ): $(IN_DIR)/bc_icpu_rcv.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_icpu_rcv.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_icpu_rcv.c
#$(OUT_DIR)/bc_cpu_init.$(OBJ): $(IN_DIR)/bc_cpu_init.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_cpu_init.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_cpu_init.c
#$(OUT_DIR)/rb_log.$(OBJ): $(IN_DIR)/rb_log.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/rb_log.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/rb_log.c
#$(OUT_DIR)/ss_rbuf.$(OBJ): $(IN_DIR)/ss_rbuf.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_rbuf.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_rbuf.c
#$(OUT_DIR)/bc_free_mgr.$(OBJ): $(IN_DIR)/bc_free_mgr.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_free_mgr.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_free_mgr.c
#$(OUT_DIR)/bc_batch_mgr.$(OBJ): $(IN_DIR)/bc_batch_mgr.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/bc_batch_mgr.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/bc_batch_mgr.c
#endif
#$(OUT_DIR)/ss_drvr.$(OBJ): $(IN_DIR)/ss_drvr.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_drvr.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_drvr.c
#$(OUT_DIR)/ss_gen.$(OBJ): $(IN_DIR)/ss_gen.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_gen.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_gen.c
#$(OUT_DIR)/ss_id.$(OBJ): $(IN_DIR)/ss_id.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_id.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_id.c
#$(OUT_DIR)/ss_mem.$(OBJ): $(IN_DIR)/ss_mem.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_mem.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_mem.c
#$(OUT_DIR)/ss_msg.$(OBJ): $(IN_DIR)/ss_msg.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_msg.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_msg.c
#$(OUT_DIR)/ss_pack.$(OBJ): $(IN_DIR)/ss_pack.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_pack.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_pack.c
#$(OUT_DIR)/ss_queue.$(OBJ): $(IN_DIR)/ss_queue.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_queue.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_queue.c
#$(OUT_DIR)/ss_rtr.$(OBJ): $(IN_DIR)/ss_rtr.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_rtr.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_rtr.c
#$(OUT_DIR)/ss_strm.$(OBJ): $(IN_DIR)/ss_strm.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_strm.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_strm.c
##$(OUT_DIR)/ss_task.$(OBJ): $(IN_DIR)/ss_task.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_task.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_task.c
#$(OUT_DIR)/ss_timer.$(OBJ): $(IN_DIR)/ss_timer.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_timer.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_timer.c
#$(OUT_DIR)/ss_diag.$(OBJ): $(IN_DIR)/ss_diag.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_diag.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_diag.c
#

lib:$(LIB_DIR)/libmt.a

include $(COM_BUILD_DIR)/compile.mak

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libmt.a:$(C_OBJS_WO_LOG)
	$(Q)ar -cr $(LIB_DIR)/libmt.a $(C_OBJS_WO_LOG)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	@echo -e "$(COLOR_RED)Cleaning SSI from $(OBJ_DIR)$(REVERT_COLOR)"
	$(Q)\rm -f $(LIB_DIR)/libmt.a $(C_OBJS_WO_LOG)


