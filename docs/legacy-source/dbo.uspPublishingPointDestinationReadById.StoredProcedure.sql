/****** Object:  StoredProcedure [dbo].[uspPublishingPointDestinationReadById]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspPublishingPointDestinationReadById] 
(
	@PublishingPointId uniqueidentifier = N'00000000-0000-0000-0000-000000000000'
)
AS 

SET NOCOUNT ON

SELECT 
	PPD.ID
	, PPD.ConferenceParticipantID AS PptParticipantId
	, PPD.ipVerLen
	, PPD.ipTos 
	, PPD.ipTotalLength
	, PPD.ipId
	, PPD.ipOffset 
	, PPD.ipTtl 
	, PPD.ipProtocol 
	, PPD.srcIP 
	, PPD.destIP 
	, PPD.srcPort
	, PPD.destPort 
	, PPD.udpLength
	, C.SessionID
	, C.ID AS PublishingPointId
	, CP.MacAddress
FROM utConferenceParticipant AS CP 
	INNER JOIN utConference  AS C 
		ON CP.ConferenceID = C.ID 
	INNER JOIN utPublishingPointDestination PPD 
		ON CP.ID = PPD.ConferenceParticipantID
WHERE PPD.ID = @PublishingPointId
GO
