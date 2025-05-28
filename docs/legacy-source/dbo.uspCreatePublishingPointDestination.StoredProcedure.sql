/****** Object:  StoredProcedure [dbo].[uspCreatePublishingPointDestination]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspCreatePublishingPointDestination] 
(
	@ID uniqueidentifier 
	, @ConferenceParticipantID uniqueidentifier 
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
) AS 
INSERT INTO utPublishingPointDestination 
(
	ID
	, ConferenceParticipantID
	, ipVerLen 
	, ipTos 
	, ipTotalLength 
	, ipId 
	, ipOffset 
	, ipTtl 
	, ipProtocol 
	, srcIP 
	, destIP 
	, srcPort 
	, destPort 
	, udpLength 
	, isActive
	, isAvailable
) VALUES 
(
	@ID
	, @ConferenceParticipantID
	, @ipVerLen 
	, @ipTos 
	, @ipTotalLength 
	, @ipId 
	, @ipOffset 
	, @ipTtl 
	, @ipProtocol 
	, @srcIP 
	, @destIP 
	, @srcPort 
	, @destPort 
	, @udpLength 
	, @isActive
	, @isAvailable
)
GO
