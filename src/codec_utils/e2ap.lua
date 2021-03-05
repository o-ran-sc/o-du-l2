----------------------------------------
-- script-name: e2ap.lua
----------------------------------------

----------------------------------------
-- creates a Proto object, but doesn't register it yet
local E2AP = Proto("E2AP","E2 Application Protocol")

----------------------------------------		
local E2apPduEnum = {
		[0] = "initiatingMessage",
		[1] = "successfulOutcome",
		[2] = "unsuccessfulOutcome" }
	
local ProcedureCodeE2 = {
		[1] = "ProcedureCodeE2_id_E2setup",
		[2] = "ProcedureCodeE2_id_ErrorIndicationE2",
		[3] = "ProcedureCodeE2_id_Reset",
		[4] = "ProcedureCodeE2_id_RICcontrol",
		[5] = "ProcedureCodeE2_id_RICindication",
		[6] = "ProcedureCodeE2_id_RICserviceQuery",
		[7] = "ProcedureCodeE2_id_RICserviceUpdate",
		[8] = "ProcedureCodeE2_id_RICsubscription",
		[9] = "ProcedureCodeE2_id_RICsubscriptionDelete" }

local CriticalityE2 ={
		[0] = "CriticalityE2_reject",
		[1] = "CriticalityE2_ignore",
		[2] = "CriticalityE2_notify" }
		
local InitiatingMessageE2ValuePrEnum = {
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
	
--payload fields start
local RIC_indication = "Ric Indication Message"

local Initiating_Message_Type_Value = {
	value = "value"--, nil,InitiatingMessageE2ValuePrEnum, 0xFF)
}

local InitiatingMessageE2 = {
	Initiating_Message_E2 = "Initiating Message E2",
	procedureCode = "ProcedureCode: ",
	criticality   = "Criticality: "
}
	--value	      = ProtoField.uint16("Initiating_Message_Type_Value.value", "value")

local InitiatingMessageE2__value_u = {
RICsubscriptionRequest		 = "RIC subscription Request",
RICsubscriptionDeleteRequest = "RIC subscription Delete Request",
RICserviceUpdate   			 = "RIC service Update",
RICcontrolRequest			 = "RIC control Request",
E2setupRequest				 = "E2 setup Request",
ResetRequest				 = "Reset Request",
RICindication				 = "RIC indication",
RICserviceQuery				 = "RIC service Query",
ErrorIndicationE2			 = "Error Indication E2"
}

local successfulOutcomeE2 = {
	successOutcomeMsg = "successfulOutcome",
	procedureCode = "procedureCode: ",
	criticality = "criticality: "
}

local e2SetupResponse = {
	e2SetupResponseMsg = "E2SetupResponse"
}

E2AP.fields = {
	--successfulOutcomeE2.procedureCode
}

function addE2apInitiatingMessageToTree(tvbuf, tree, offset, endianness)

	local info = tvbuf:range(offset, 1)
	local initiatingMessageE2Tree = tree:add(info, InitiatingMessageE2.Initiating_Message_E2)

        local procId = tvbuf:range(offset,1):le_uint()
        strId = tostring(ProcedureCodeE2[procId]) .. " (" .. tostring(procId) ..")"
        tree:add(tvbuf:range(offset,1), InitiatingMessageE2.procedureCode .. strId)
        offset = offset + 1

	local criticalityId = tvbuf:range(offset,1):le_uint()
        strId = tostring(CriticalityE2[criticalityId]) .. " (" .. tostring(criticalityId) ..")"
        tree:add(tvbuf:range(offset,1), InitiatingMessageE2.criticality .. strId)
        offset = offset + 1

	-- value cfg
	--local valueinitiatingMessageType = initiatingMessageE2Tree:add(tvbuf:range(offset,offset),Initiating_Message_E2.value)

end

function addE2SetupResponseToTree(tvbuf, tree, offset, pktlen, endianness)
	local valueTree = tree:add(tvbuf:range(offset, (pktlen - offset)), "value")
		local e2SetupResponseTree = valueTree:add(e2SetupResponse.e2SetupResponseMsg)
			
			local extBit = tvbuf:range(offset,1):le_uint()
			if(extBit == 0) then
				e2SetupResponseTree:add(tvbuf:range(offset,1), "<" .. extBit .. "... .... Extension Bit: FALSE>")
			else
				e2SetupResponseTree:add(tvbuf:range(offset,1), "<" .. extBit .. "... .... Extension Bit: TRUE>")
			end
			offset = offset + 1
			
			local seqLen = tvbuf:range(offset, 2):le_uint()
			e2SetupResponseTree:add(tvbuf:range(offset, 2), "<Sequence-Of Length: " .. seqLen .. ">")
			offset = offset + 2
			
end

function addE2apSuccessfulOutcomeToTree(tvbuf, tree, offset, pktlen, endianness)
	local successfulOutcomeTree = tree:add(tvbuf:range(offset, (pktlen-offset)), successfulOutcomeE2.successOutcomeMsg)
		local procId = tvbuf:range(offset,1):le_uint()
		strId = tostring(ProcedureCodeE2[procId]) .. " (" .. tostring(procId) ..")"
		successfulOutcomeTree:add(tvbuf:range(offset,1), successfulOutcomeE2.procedureCode .. strId)
		offset = offset + 1
		
		local criticalityId = tvbuf:range(offset,1):le_uint()
		successfulOutcomeTree:add(tvbuf:range(offset,1), "<Enumerated Index: " .. criticalityId ..">")
        strId = tostring(CriticalityE2[criticalityId]) .. " (" .. tostring(criticalityId) ..")"
        successfulOutcomeTree:add(tvbuf:range(offset,1), successfulOutcomeE2.criticality .. strId)
        offset = offset + 1
		
		local valueLen = tvbuf:range(offset,1):le_uint()
		successfulOutcomeTree:add(tvbuf:range(offset,1), "<Value length: " .. valueLen ..">")
		offset = offset + 1
		
		if(procId == 1) then
			addE2SetupResponseToTree(tvbuf, successfulOutcomeTree, offset, pktlen, endianness)
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
	tree:add(tvbuf:range(offset,1), "<Choice Index: " .. apiId ..">")
    strId = tostring(E2apPduEnum[apiId]) .. " (" .. tostring(apiId) ..")"
    tree:add(tvbuf:range(offset,1), "E2AP-PDU: " .. strId)
	offset = offset + 1

    -- Now let's add our protocol fields under the protocol tree we just created.
	if (apiId == 0) then
		addE2apInitiatingMessageToTree(tvbuf, tree, offset, endianness)
	end
	if(apiId == 1) then
		addE2apSuccessfulOutcomeToTree(tvbuf, tree, offset, pktlen, endianness)
	end
	
end -- end of dissector funciton

	----------------------------------------
	-- we want to have our protocol dissection invoked for a specific UDP port,
	-- so get the udp dissector table and add our protocol to it
	DissectorTable.get("sctp.port"):add(36421, E2AP)
	-- our protocol (Proto) gets automatically registered after this script finishes loading
	----------------------------------------

