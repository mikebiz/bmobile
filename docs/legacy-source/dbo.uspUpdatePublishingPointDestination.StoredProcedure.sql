/****** Object:  StoredProcedure [dbo].[uspUpdatePublishingPointDestination]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspUpdatePublishingPointDestination] 
(
	@ID uniqueidentifier 
	,@ConferenceParticipantID uniqueidentifier 
	, @ipVerLen smallint
	, @ipTos smallint
	, @ipTotalLength int
	, @ipId int
	, @ipOffset int
	, @ipTtl smallint
	, @ipProtocol smallint
	, @srcIP bigint
	, @destIP bigint
	, @srcPort int
	, @destPort int
	, @udpLength int
	, @isActive bit
	, @isAvailable bit
) 
AS
 
UPDATE utPublishingPointDestination 
SET ipVerLen = @ipVerLen
	, ipTos = @ipTos
	, ipTotalLength = @ipTotalLength
	, ipId = @ipId
	, ipOffset = @ipOffset
	, ipTtl = @ipTtl
	, ipProtocol = @ipProtocol
	, srcIP = @srcIP
	, destIP = @destIP
	, srcPort = @srcPort
	, destPort = @destPort
	, udpLength = @udpLength
	, isActive = @isActive
	, isAvailable = @isAvailable
	,ConferenceParticipantID=@ConferenceParticipantID 
WHERE ID=@ID
GO
