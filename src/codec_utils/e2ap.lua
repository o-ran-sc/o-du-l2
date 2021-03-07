-------------------------------------------------------------------------------------
-- script-name: e2ap.lua
-- This script is a plugin to dissect packets that are exchanged over E2AP interface
-------------------------------------------------------------------------------------

----------------------------------------
-- creates a Proto object, but doesn't register it yet
local E2AP = Proto("E2AP","E2 Application Protocol")

----------------------------------------		
local E2apPduEnum = {
		[0] = "initiatingMessage",
		[1] = "successfulOutcome",
		[2] = "unsuccessfulOutcome" }
	
local E2ProcedureCode = {
		[1] = "ProcedureCodeE2_id_E2setup",
		[2] = "ProcedureCodeE2_id_ErrorIndicationE2",
		[3] = "ProcedureCodeE2_id_Reset",
		[4] = "ProcedureCodeE2_id_RICcontrol",
		[5] = "ProcedureCodeE2_id_RICindication",
		[6] = "ProcedureCodeE2_id_RICserviceQuery",
		[7] = "ProcedureCodeE2_id_RICserviceUpdate",
		[8] = "ProcedureCodeE2_id_RICsubscription",
		[9] = "ProcedureCodeE2_id_RICsubscriptionDelete" }

local E2Criticality ={
		[0] = "CriticalityE2_reject",
		[1] = "CriticalityE2_ignore",
		[2] = "CriticalityE2_notify" }
		
local E2InitiatingMessagePresenceEnum = {
		[0] = "InitiatingMessageE2__value_PR_NOTHING",
		[1] = "InitiatingMessageE2__value_PR_RICsubscriptionRequest",
		[2] = "InitiatingMessageE2__value_PR_RICsubscriptionDeleteRequest",
		[3] = "InitiatingMessageE2__value_PR_RICserviceUpdate",
		[4] = "InitiatingMessageE2__value_PR_RICcontrolRequest",
		[5] = "InitiatingMessageE2__value_PR_E2setupRequest",
		[6] = "InitiatingMessageE2__value_PR_ResetRequest",
		[7] = "InitiatingMessageE2__value_PR_RICindication",
		[8] = "InitiatingMessageE2__value_PR_RICserviceQuery",
		[9] = "InitiatingMessageE2__value_PR_ErrorIndicationE2" }

local E2PotocolIEs = {
	[1] = "id_CauseE2",
	[2] = "id_CriticalityDiagnosticsE2",
	[3] = "id_GlobalE2node_ID",
	[4] = "id_GlobalRIC_ID",
	[5] = "id_RANfunctionID",
	[6] = "id_RANfunctionID_Item",
	[7] = "id_RANfunctionIEcause_Item",
	[8] = "id_RANfunction_Item",
	[9] = "id_RANfunctionsAccepted",
	[10] = "id_RANfunctionsAdded",
	[11] = "id_RANfunctionsDeleted",
	[12] = "id_RANfunctionsModified",
	[13] = "id_RANfunctionsRejected",
	[14] = "id_RICaction_Admitted_Item",
	[15] = "id_RICactionID",
	[16] = "id_RICaction_NotAdmitted_Item",
	[17] = "id_RICactions_Admitted",
	[18] = "id_RICactions_NotAdmitted",
	[19] = "id_RICaction_ToBeSetup_Item",
	[20] = "id_RICcallProcessID",
	[21] = "id_RICcontrolAckRequest",
	[22] = "id_RICcontrolHeader",
	[23] = "id_RICcontrolMessage",
	[24] = "id_RICcontrolStatus",
	[25] = "id_RICindicationHeader",
	[26] = "id_RICindicationMessage",
	[27] = "id_RICindicationSN",
	[28] = "id_RICindicationType",
	[29] = "id_RICrequestID",
	[30] = "id_RICsubscriptionDetails",
	[31] = "id_TimeToWaitE2",
	[32] = "id_RICcontrolOutcome"
}

	
--payload fields start
local RIC_indication = "Ric Indication Message"

local Initiating_Message_Type_Value = {
	value = "value"--, nil,E2InitiatingMessagePresenceEnum, 0xFF)
}

local InitiatingMessageE2__value_u = {
RICsubscriptionRequest		 = "RIC subscription Request",
RICsubscriptionDeleteRequest     = "RIC subscription Delete Request",
RICserviceUpdate   		 = "RIC service Update",
RICcontrolRequest		 = "RIC control Request",
E2setupRequest			 = "E2 setup Request",
ResetRequest			 = "Reset Request",
RICindication			 = "RIC indication",
RICserviceQuery			 = "RIC service Query",
ErrorIndicationE2		 = "Error Indication E2"
}

local initiatingMessageE2 = {
   initiatingMsg = "Initiating Message",
	procedureCode = "procedureCode: ",
	criticality = "criticality: ",
	valueLen = "Value Length: ",
	value = "value"
}
local successfulOutcomeE2 = {
	successOutcomeMsg = "successfulOutcome",
	procedureCode = "procedureCode: ",
	criticality = "criticality: ",
	valueLen = "Value Length: ",
	value = "value"
}

local protocolIE = "protocolIEs: "
local protocolItem = "Item "
local protocolIEField = {
	protocolIEFieldMsg = "ProtocolIE-Field",
	id = "id: ",
	criticality = "criticality: ",
	valueLen = "Value Length: ",
	value = "value"
}

local e2SetupResponse = "E2SetupResponse"
local ricSubscriptionReq = "E2RicSubscriptionRequest"
local ricSubscriptionRsp = "E2RicSubscriptionResponse"

local globalRicId = {
	globalRicIdMsg = "Global RIC-Id",
	plmnId = "PLMN-Identity: ",
	ricId = "RIC-Id: "
}

local RICRequestId = {
	ricRequestId = "RICRequestID",
	ricRequestorID = "ricRequestorID",
	ricInstanceID = "ricInstanceID"
}

local RANfunctionID = "RANfunctionID"

local RICactionAdmittedList = {
	ricActionAdmittedList = "RICaction-Admitted-List",
	seqLen = "Sequence-Of Length: ",
	protocolIEContainer = "ProtocolIE-SingleContainerE2: "
}

local RICactionAdmittedItem = {
	ricActionAdmittedItem = "RICaction-Admitted-Item",
	ricActionID = "ricActionID: "
}
local RICsubscriptionDetailsList = {
	ricSubscriptionDetailsList = "RICsubscription-Detais-List",
   ricEventTriggerDefinition = "ricEventTriggerDefinition: "
}

local RICactionToBeSetupList = {
   ricActionSetupList = "RICaction-ToBeSetup-List",
	seqLen = "Sequence-Of Length: ",
	protocolIEContainer = "ProtocolIE-SingleContainerE2: "
}

local RICactionToBeSetupItem = {
   ricActionToBeSetupItem = "RICaction-ToBeSetup-Item",
	ricActionID    = "ricActionID: ",
	ricActionType  = "ricActionType: "
}

E2AP.fields = {
}

function fetch2Bytes(tvbuf, offset)
	local fbyte = tvbuf:range(offset, 1):le_uint()
	local fbyte1 = bit.lshift(fbyte, 8)
	local sbyte = tvbuf:range(offset + 1, 1):le_uint()
	local result = bit.bor(fbyte1, sbyte)
	return result
end

function convertBin(n)
    local t = {}
    for i = 1, 8 do
        n = bit.rol(n, 1)
        table.insert(t, bit.band(n, 1))
    end
    return table.concat(t)
end



function addE2apInitiatingMessageToTree(tvbuf, pktinfo, tree, offset, pktlen, endianness)

        local initiatingMessageTree = tree:add(tvbuf:range(offset, (pktlen-offset)), initiatingMessageE2.initiatingMsg)

	local procId = tvbuf:range(offset,1):le_uint()
	strId = tostring(E2ProcedureCode[procId]) .. " (" .. tostring(procId) ..")"
	initiatingMessageTree:add(tvbuf:range(offset,1), initiatingMessageE2.procedureCode .. strId)
	offset = offset + 1
	
	local fbyte = tvbuf:range(offset,1):le_uint()
	local criticalityId = bit.rshift(fbyte, 6)
        strId = tostring(E2Criticality[criticalityId]) .. " (" .. tostring(criticalityId) ..")"
        initiatingMessageTree:add(tvbuf:range(offset,1), initiatingMessageE2.criticality .. strId)
        offset = offset + 1
		
	local valueLen = tvbuf:range(offset,1):le_uint()
	offset = offset + 1
	
	local valueTree = initiatingMessageTree:add(tvbuf:range(offset, valueLen), initiatingMessageE2.value)
	
	if(procId == 8) then
		addE2RicSubscriptionReqToTree(tvbuf, pktinfo, valueTree, offset, valueLen, endianness)
	end
end

function addE2RicSubscriptionReqToTree(tvbuf, pktinfo, valueTree, offset, valueLen, endianness)
	pktinfo.cols.info:set("E2 RIC Subscription Request")

	local e2RicSubscriptionReqTree = valueTree:add(tvbuf:range(offset, valueLen), ricSubscriptionReq)
	offset = offset + 1
		
	local seqLen = fetch2Bytes(tvbuf, offset)
	local protoIeTree = e2RicSubscriptionReqTree:add(tvbuf:range(offset, 2), protocolIE .. seqLen .. " items")
	offset = offset + 2

        for ie=0, seqLen-1, 1
	do
	    local protocolIEId = fetch2Bytes(tvbuf, offset)
	    strId = tostring(E2PotocolIEs[protocolIEId])
	    local itemTree = protoIeTree:add(protocolItem .. ie .. ": " .. strId)
		
            strId = strId .. " (" .. tostring(protocolIEId) ..")"
	    local protocolFieldTree = itemTree:add(protocolIEField.protocolIEFieldMsg)
		protocolFieldTree:add(tvbuf:range(offset,2), protocolIEField.id .. strId)
		offset = offset + 2
			
	        local fbyte = tvbuf:range(offset,1):le_uint()
	        local criticalityId = bit.rshift(fbyte, 6)
		strId = tostring(E2Criticality[criticalityId]) .. " (" .. tostring(criticalityId) ..")"
		protocolFieldTree:add(tvbuf:range(offset,1), protocolIEField.criticality .. strId)
		offset = offset + 1
		
		local valueLen = tvbuf:range(offset,1):le_uint()
		offset = offset + 1
		
		local valueTree = protocolFieldTree:add(tvbuf:range(offset, valueLen), protocolIEField.value)
		
		if(protocolIEId == 29) then
			addRICRequestIdToTree(tvbuf, valueTree, offset, valueLen, endianness)
			offset = offset + valueLen
		end
		
		if(protocolIEId == 5) then
			addRANFunctionIdToTree(tvbuf, valueTree, offset, valueLen, endianness)
			offset = offset + valueLen
		end
		
		if(protocolIEId == 30) then
			addRICSubscriptionDetailsToTree(tvbuf, valueTree, offset, valueLen, endianness)
			offset = offset + valueLen
		end
		
	end

end

function addRICSubscriptionDetailsToTree(tvbuf, valueTree, offset, valueLen, endianness)

	local ricSubscriptionTree = valueTree:add(tvbuf:range(offset, valueLen), RICsubscriptionDetailsList.ricSubscriptionDetailsList)
	offset = offset + 1

	local eventTrigger = tvbuf:range(offset,3)
	ricSubscriptionTree:add(tvbuf:range(offset,3), RICsubscriptionDetailsList.ricEventTriggerDefinition ..
	eventTrigger)
	offset = offset + 3
	offset = offset + 1 --spare

	local ricActionSetupListTree = ricSubscriptionTree:add(tvbuf:range(offset, valueLen-offset), RICactionToBeSetupList.ricActionSetupList)

        local fbyte = tvbuf:range(offset,1):le_uint()
	local seqLen = bit.rshift(fbyte, 4)
	seqLen = seqLen + 1

	local protoIeTree = ricActionSetupListTree:add(tvbuf:range(offset, 1), protocolIE .. seqLen .. " items")
	offset = offset+ 1

        for ie=0, seqLen-1, 1
	do
	    local protocolIEId = fetch2Bytes(tvbuf, offset)
	    strId = tostring(E2PotocolIEs[protocolIEId])
	    local itemTree = protoIeTree:add(protocolItem .. ie .. ": " .. strId)
			
	    strId = strId .. " (" .. tostring(protocolIEId) ..")"
	    local protocolFieldTree = itemTree:add(tvbuf:range(offset,2), protocolIEField.protocolIEFieldMsg)
		protocolFieldTree:add(protocolIEField.id .. strId)
		offset = offset + 2
			
		local fbyte = tvbuf:range(offset,1):le_uint()
		local criticalityId = bit.rshift(fbyte, 6)
		strId = tostring(E2Criticality[criticalityId]) .. " (" .. tostring(criticalityId) ..")"
		protocolFieldTree:add(tvbuf:range(offset,1), protocolIEField.criticality .. strId)
		offset = offset + 1
		
		local valueLen = tvbuf:range(offset,1):le_uint()
		offset = offset + 1
		
		local valueTree = protocolFieldTree:add(tvbuf:range(offset, valueLen), protocolIEField.value)
		
		if(protocolIEId == 19) then
			addRICActionItemToTree(tvbuf, valueTree, offset, valueLen, endianness)
		end
         end
end

function addRICActionItemToTree(tvbuf, valueTree, offset, valueLen, endianness)
    local ricActionItemTree = valueTree:add(tvbuf:range(offset, valueLen), RICactionToBeSetupItem.ricActionToBeSetupItem)
    offset = offset + 1

    local actionId = tvbuf:range(offset,1):le_uint()
	ricActionItemTree:add(tvbuf:range(offset,1), RICactionToBeSetupItem.ricActionID .. actionId)
	offset = offset + 1

    local fbyte = tvbuf:range(offset,1):le_uint()
    local actionType = bit.rshift(fbyte, 4)
	ricActionItemTree:add(tvbuf:range(offset,1), RICactionToBeSetupItem.ricActionType .. actionType)
	offset = offset + 1

end

function addGobalRICIdToTree(tvbuf, valueTree, offset, valueLen, endianness)
	local gRicIdTree = valueTree:add(tvbuf:range(offset, valueLen), globalRicId.globalRicIdMsg)
		--local extBit = tvbuf:range(offset,1):le_uint()
		--if(extBit == 0) then
		--	gRicIdTree:add(tvbuf:range(offset,1), "<" .. extBit .. "... .... Extension Bit: FALSE>")
		--else
		--	gRicIdTree:add(tvbuf:range(offset,1), "<" .. extBit .. "... .... Extension Bit: TRUE>")
		--end
		offset = offset + 1
	
		local plmnId = tvbuf:range(offset,3)
		gRicIdTree:add(tvbuf:range(offset,3), globalRicId.plmnId .. plmnId)
		offset = offset + 3
	
		local ricIdByte1 = tvbuf:range(offset,1):le_uint()
		local ricId1 = convertBin(ricIdByte1)
		local ricIdByte2 = tvbuf:range(offset+1,1):le_uint()
		local ricId2 = convertBin(ricIdByte2)
		local ricIdByte3 = tvbuf:range(offset+2,1):le_uint()
		local ricId3 = convertBin(ricIdByte3)
		gRicIdTree:add(tvbuf:range(offset,3), globalRicId.ricId .. ricId1 .. ricId2 .. ricId3)
		offset = offset + 3
end

function addE2SetupResponseToTree(tvbuf, pktinfo, valueTree, offset, pktlen, endianness)
    pktinfo.cols.info:set("E2 Setup Response")

	local e2SetupResponseTree = valueTree:add(tvbuf:range(offset, pktlen), e2SetupResponse)
			
--        local extBit = tvbuf:range(offset,1):le_uint()
--	if(extBit == 0) then
--		e2SetupResponseTree:add(tvbuf:range(offset,1), "<" .. extBit .. "... .... Extension Bit: FALSE>")
--	else
--		e2SetupResponseTree:add(tvbuf:range(offset,1), "<" .. extBit .. "... .... Extension Bit: TRUE>")
--	end
	offset = offset + 1
		
	local seqLen = fetch2Bytes(tvbuf, offset)
--	e2SetupResponseTree:add(tvbuf:range(offset, 2), "<Sequence-Of Length: " .. seqLen .. ">")
	local protoIeTree = e2SetupResponseTree:add(tvbuf:range(offset, 2), protocolIE .. seqLen .. " items")
	offset = offset + 2
			
	for ie=0, seqLen-1, 1
	do
	    local protocolIEId = fetch2Bytes(tvbuf, offset)
	    strId = tostring(E2PotocolIEs[protocolIEId])
	    local itemTree = protoIeTree:add(protocolItem .. ie .. ": " .. strId)
			
	    strId = strId .. " (" .. tostring(protocolIEId) ..")"
	    local protocolFieldTree = itemTree:add(tvbuf:range(offset,2), protocolIEField.protocolIEFieldMsg)
		protocolFieldTree:add(protocolIEField.id .. strId)
		offset = offset + 2
			
	   local fbyte = tvbuf:range(offset,1):le_uint()
	   local criticalityId = bit.rshift(fbyte, 6)
		strId = tostring(E2Criticality[criticalityId]) .. " (" .. tostring(criticalityId) ..")"
		protocolFieldTree:add(tvbuf:range(offset,1), protocolIEField.criticality .. strId)
		offset = offset + 1
		
		local valueLen = tvbuf:range(offset,1):le_uint()
--		protocolFieldTree:add(tvbuf:range(offset,1), "<" .. protocolIEField.valueLen .. valueLen ..">")
		offset = offset + 1
		
		local valueTree = protocolFieldTree:add(tvbuf:range(offset, valueLen), protocolIEField.value)
		
		if(protocolIEId == 4) then
			addGobalRICIdToTree(tvbuf, valueTree, offset, valueLen, endianness)
		end
	end
end

function addRICRequestIdToTree(tvbuf, valueTree, offset, valueLen, endianness)
	local RICReqIdTree = valueTree:add(tvbuf:range(offset, valueLen), RICRequestId.ricRequestId)
		--local extBit = tvbuf:range(offset,1):le_uint()
		--if(extBit == 0) then
		--	RICReqIdTree:add(tvbuf:range(offset,1), "<" .. extBit .. "... .... Extension Bit: FALSE>")
		--else
		--	RICReqIdTree:add(tvbuf:range(offset,1), "<" .. extBit .. "... .... Extension Bit: TRUE>")
		--end
		offset = offset + 1
	
		local ricReqID = fetch2Bytes(tvbuf, offset)
		RICReqIdTree:add(tvbuf:range(offset,2), RICRequestId.ricRequestorID .. ": " .. ricReqID)
		offset = offset + 2
		
		local ricInstID = fetch2Bytes(tvbuf, offset)
		RICReqIdTree:add(tvbuf:range(offset,2), RICRequestId.ricInstanceID .. ": " .. ricInstID)
		offset = offset + 2
end

function addRANFunctionIdToTree(tvbuf, valueTree, offset, valueLen, endianness)		
		local ranFuncID = fetch2Bytes(tvbuf, offset)
		valueTree:add(tvbuf:range(offset,2), RANfunctionID .. ": " .. ranFuncID)
		offset = offset + 2
end

function addRICActionAdmittedListToTree(tvbuf, valueTree, offset, valueLen, endianness)
	local RICActionAdmittedListTree = valueTree:add(tvbuf:range(offset, valueLen), RICactionAdmittedList.ricActionAdmittedList)
		local fbyte = tvbuf:range(offset,1):le_uint()
		local seqLen = bit.rshift(fbyte, 4)
		seqLen = seqLen + 1
		--RICActionAdmittedListTree:add(tvbuf:range(offset, 1), "<" .. RICactionAdmittedList.seqLen .. seqLen .. ">")
		local protoIeTree = RICActionAdmittedListTree:add(tvbuf:range(offset, 1), protocolIE .. seqLen .. " items")
		offset = offset+1
		
		for ie=0, seqLen-1, 1
		do
			local protocolIEId = fetch2Bytes(tvbuf, offset)
			strId = tostring(E2PotocolIEs[protocolIEId])
			local itemTree = protoIeTree:add(protocolItem .. ie .. ": " .. strId)
		
			strId = strId .. " (" .. tostring(protocolIEId) ..")"
			local protocolFieldTree = itemTree:add(RICactionAdmittedList.protocolIEContainer)
			protocolFieldTree:add(tvbuf:range(offset,2), protocolIEField.id .. strId)
			offset = offset + 2
			
	      local fbyte = tvbuf:range(offset,1):le_uint()
	      local criticalityId = bit.rshift(fbyte, 6)
--			protocolFieldTree:add(tvbuf:range(offset,1), "<Enumerated Index: " .. criticalityId ..">")
			strId = tostring(E2Criticality[criticalityId]) .. " (" .. tostring(criticalityId) ..")"
			protocolFieldTree:add(tvbuf:range(offset,1), protocolIEField.criticality .. strId)
			offset = offset + 1
		
			local valueLen = tvbuf:range(offset,1):le_uint()
--			protocolFieldTree:add(tvbuf:range(offset,1), "<" .. protocolIEField.valueLen .. valueLen ..">")
			offset = offset + 1
		
			local valueTree = protocolFieldTree:add(tvbuf:range(offset, valueLen), protocolIEField.value)
		
			if(protocolIEId == 14) then
				local ricActionItemTree = valueTree:add(tvbuf:range(offset, valueLen), RICactionAdmittedItem.ricActionAdmittedItem)
				local actionId = fetch2Bytes(tvbuf, offset)
				ricActionItemTree:add(tvbuf:range(offset, valueLen), RICactionAdmittedItem.ricActionID .. actionId)
				offset = offset + valueLen
			end
		end
end

function addE2RicSubscriptionRspToTree(tvbuf, pktinfo, valueTree, offset, valueLen, endianness)
	pktinfo.cols.info:set("E2 RIC Subscription Response")

	local e2RicSubscriptionRspTree = valueTree:add(tvbuf:range(offset, valueLen), ricSubscriptionRsp)
			
--        local extBit = tvbuf:range(offset,1):le_uint()
--	if(extBit == 0) then
--		e2RicSubscriptionRspTree:add(tvbuf:range(offset,1), "<" .. extBit .. "... .... Extension Bit: FALSE>")
--	else
--		e2RicSubscriptionRspTree:add(tvbuf:range(offset,1), "<" .. extBit .. "... .... Extension Bit: TRUE>")
--	end
	offset = offset + 1
		
	local seqLen = fetch2Bytes(tvbuf, offset)
--	e2RicSubscriptionRspTree:add(tvbuf:range(offset, 2), "<Sequence-Of Length: " .. seqLen .. ">")
	local protoIeTree = e2RicSubscriptionRspTree:add(tvbuf:range(offset, 2), protocolIE .. seqLen .. " items")
	offset = offset + 2
			
	for ie=0, seqLen-1, 1
	do
	    local protocolIEId = fetch2Bytes(tvbuf, offset)
	    strId = tostring(E2PotocolIEs[protocolIEId])
	    local itemTree = protoIeTree:add(protocolItem .. ie .. ": " .. strId)
		
		strId = strId .. " (" .. tostring(protocolIEId) ..")"
	   local protocolFieldTree = itemTree:add(protocolIEField.protocolIEFieldMsg)
		protocolFieldTree:add(tvbuf:range(offset,2), protocolIEField.id .. strId)
		offset = offset + 2
			
	   local fbyte = tvbuf:range(offset,1):le_uint()
	   local criticalityId = bit.rshift(fbyte, 6)
--		protocolFieldTree:add(tvbuf:range(offset,1), "<Enumerated Index: " .. criticalityId ..">")
		strId = tostring(E2Criticality[criticalityId]) .. " (" .. tostring(criticalityId) ..")"
		protocolFieldTree:add(tvbuf:range(offset,1), protocolIEField.criticality .. strId)
		offset = offset + 1
		
		local valueLen = tvbuf:range(offset,1):le_uint()
--		protocolFieldTree:add(tvbuf:range(offset,1), "<" .. protocolIEField.valueLen .. valueLen ..">")
		offset = offset + 1
		
		local valueTree = protocolFieldTree:add(tvbuf:range(offset, valueLen), protocolIEField.value)
		
		if(protocolIEId == 29) then
			addRICRequestIdToTree(tvbuf, valueTree, offset, valueLen, endianness)
			offset = offset + valueLen
		end
		
		if(protocolIEId == 5) then
			addRANFunctionIdToTree(tvbuf, valueTree, offset, valueLen, endianness)
			offset = offset + valueLen
		end
		
		if(protocolIEId == 17) then
			addRICActionAdmittedListToTree(tvbuf, valueTree, offset, valueLen, endianness)
			offset = offset + valueLen
		end
		
	end
end

function addE2apSuccessfulOutcomeToTree(tvbuf, pktinfo, tree, offset, pktlen, endianness)
	local successfulOutcomeTree = tree:add(tvbuf:range(offset, (pktlen-offset)), successfulOutcomeE2.successOutcomeMsg)
	local procId = tvbuf:range(offset,1):le_uint()
	strId = tostring(E2ProcedureCode[procId]) .. " (" .. tostring(procId) ..")"
	successfulOutcomeTree:add(tvbuf:range(offset,1), successfulOutcomeE2.procedureCode .. strId)
	offset = offset + 1
	
	local fbyte = tvbuf:range(offset,1):le_uint()
	local criticalityId = bit.rshift(fbyte, 6)
--	successfulOutcomeTree:add(tvbuf:range(offset,1), "<Enumerated Index: " .. criticalityId ..">")
        strId = tostring(E2Criticality[criticalityId]) .. " (" .. tostring(criticalityId) ..")"
        successfulOutcomeTree:add(tvbuf:range(offset,1), successfulOutcomeE2.criticality .. strId)
        offset = offset + 1
		
	local valueLen = tvbuf:range(offset,1):le_uint()
--	successfulOutcomeTree:add(tvbuf:range(offset,1), "<" .. successfulOutcomeE2.valueLen .. valueLen ..">")
	offset = offset + 1
	
	local valueTree = successfulOutcomeTree:add(tvbuf:range(offset, valueLen), successfulOutcomeE2.value)
	
	if(procId == 1) then
		addE2SetupResponseToTree(tvbuf, pktinfo, valueTree, offset, valueLen, endianness)
	end
	if(procId == 8) then
		addE2RicSubscriptionRspToTree(tvbuf, pktinfo, valueTree, offset, valueLen, endianness)
	end
end
----------------------------------------
-- The following creates the callback function for the dissector.
-- The 'tvbuf' is a Tvb object, 'pktinfo' is a Pinfo object, and 'root' is a TreeItem object.
-- Whenever Wireshark dissects a packet that our Protocol is hooked into, it will call
-- this function and pass it these arguments for the packet it's dissecting.
function E2AP.dissector(tvbuf,pktinfo,root)

    -- set the protocol column to show our protocol name
    pktinfo.cols.protocol:set("E2AP")

    -- We want to check that the packet size is rational during dissection, so let's get the length of the
    -- packet buffer (Tvb).
    local pktlen = tvbuf:reported_length_remaining()

    -- We start by adding our protocol to the dissection display tree.
    -- A call to tree:add_packet_field() returns the child created, so we can add more "under" it using that return value.
    -- The second argument is how much of the buffer/packet this added tree item covers/represents
    local offset = 0
    stream = " "
    local endianness = string.byte(string.dump(function() end), 7)
    local tree = root:add(E2AP, tvbuf:range(offset,pktlen))

       
    -- Now let's add our protocol header fields under the protocol tree we just created.

    local fByte = tvbuf:range(offset,1):le_uint()
    local apiId = bit.rshift(fByte, 5)
--    tree:add(tvbuf:range(offset,1), "<Choice Index: " .. apiId ..">")
    strId = tostring(E2apPduEnum[apiId]) .. " (" .. tostring(apiId) ..")"
    local e2apTree = tree:add(tvbuf:range(offset,1), "E2AP-PDU: " .. strId)
	offset = offset + 1

    -- Now let's add our protocol fields under the protocol tree we just created.
	if (apiId == 0) then
		addE2apInitiatingMessageToTree(tvbuf, pktinfo, e2apTree, offset, pktlen, endianness)
	end
	if(apiId == 1) then
		addE2apSuccessfulOutcomeToTree(tvbuf, pktinfo, e2apTree, offset, pktlen, endianness)
	end
	
end -- end of dissector funciton

	----------------------------------------
	-- we want to have our protocol dissection invoked for a specific UDP port,
	-- so get the udp dissector table and add our protocol to it
	DissectorTable.get("sctp.port"):add(36421, E2AP)
	-- our protocol (Proto) gets automatically registered after this script finishes loading
	----------------------------------------

