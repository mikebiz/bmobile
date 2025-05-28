/****** Object:  StoredProcedure [dbo].[uspPublishingPointSourceReadById]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspPublishingPointSourceReadById] 
(
	@PublishingPointId uniqueidentifier = N'00000000-0000-0000-0000-000000000000'
)
AS 

SET NOCOUNT ON

SELECT 
	PPS.ID 
	, PPS.ConferenceParticipantID AS PptParticipantId
	, PPS.avSourceType
	, PPS.ipVerLen
	, PPS.ipTos 
	, PPS.ipTotalLength
	, PPS.ipId
	, PPS.ipOffset 
	, PPS.ipTtl
	, PPS.ipProtocol 
	, PPS.srcIP 
	, PPS.destIP 
	, PPS.srcPort 
	, PPS.destPort 
	, PPS.udpLength 
	, PPS.majorType 
	, PPS.subType 
	, PPS.fixedSizeSamples
	, PPS.temporalCompression 
	, PPS.sampleSize 
	, PPS.headerFormatType
	, PPS.headerFormatSize 
	, PPS.rcSourceLeft
	, PPS.rcSourceTop 
	, PPS.rcSourceRight 
	, PPS.rcSourceBottom 
	, PPS.rcTargetLeft 
	, PPS.rcTargetTop 
	, PPS.rcTargetRight 
	, PPS.rcTargetBottom 
	, PPS.dwBitRate 
	, PPS.dwBitErrorRate 
	, PPS.AvgTimePerFrame 
	, PPS.biSize
	, PPS.biHeight
	, PPS.biWidth
	, PPS.biPlanes 
	, PPS.biBitCount 
	, PPS.biCompression 
	, PPS.biSizeImage 
	, PPS.biXPelsPerMeter
	, PPS.biYPelsPerMeter
	, PPS.biClrUsed 
	, PPS.biClrImportant 
	, PPS.wFormatTag 
	, PPS.nChannels 
	, PPS.nSamplesPerSec 
	, PPS.nAvgBytesPerSec 
	, PPS.nBlockAlign 
	, PPS.wBitsPerSample
	, PPS.cbSize
	, C.SessionID
	, C.ID AS PublishingPointId
	, CP.MacAddress
FROM utConferenceParticipant AS CP 
	INNER JOIN utConference  AS C 
		ON CP.ConferenceID = C.ID 
	INNER JOIN utPublishingPointSource PPS 
		ON CP.ID = PPS.ConferenceParticipantID
WHERE PPS.ID = @PublishingPointId
GO
