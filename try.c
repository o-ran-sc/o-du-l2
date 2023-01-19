
for(slotIdx =0 ;slotIdx < MAX_TDD_PERIODICITY_SLOTS; slotIdx++) 
{
   for(symbolIdx = 0 ; symbolIdx < MAX_SYMB_PER_SLOT; symbolIdx++)
   {
      if((slotIdx < macCfgParams.tddCfg.nrOfDlSlots) ||    \
          (slotIdx == macCfgParams.tddCfg.nrOfDlSlots && symbolIdx < macCfgParams.tddCfg.nrOfDlSymbols))
      {
         fillTlvs(&configReq->tlvs[index++], FAPI_SLOT_CONFIG_TAG,               \
               sizeof(uint8_t), DL_SLOT, &msgLen);
      }
      else if(slotIdx < (MAX_TDD_PERIODICITY_SLOTS - macCfgParams.tddCfg.nrOfUlSlots) || \
               ((slotIdx == (MAX_TDD_PERIODICITY_SLOTS - macCfgParams.tddCfg.nrOfUlSlots)) && \  
                  symbolIdx < (MAX_SYMB_PER_SLOT - macCfgParams.tddCfg.nrOfUlSymbols)))
      {
         fillTlvs(&configReq->tlvs[index++], FAPI_SLOT_CONFIG_TAG,               \
               sizeof(uint8_t), FLEXI_SLOT, &msgLen);
      }
      else
      {
         fillTlvs(&configReq->tlvs[index++], FAPI_SLOT_CONFIG_TAG,               \
               sizeof(uint8_t), UL_SLOT, &msgLen);
      }
   }
}
