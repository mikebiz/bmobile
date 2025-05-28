/****** Object:  StoredProcedure [dbo].[uspCreatePublishingPointSource]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspCreatePublishingPointSource] 
(
	@ID uniqueidentifier 
	,@ConferenceParticipantID uniqueidentifier 
	,@avSourceType int
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
	,@majorType uniqueidentifier 
	,@subType uniqueidentifier 
	,@fixedSizeSamples int 
	,@temporalCompression int 
	,@sampleSize bigint 
	,@headerFormatType uniqueidentifier 
	,@headerFormatSize bigint 
	,@rcSourceLeft bigint 
	,@rcSourceTop bigint 
	,@rcSourceRight bigint 
	,@rcSourceBottom bigint 
	,@rcTargetLeft bigint 
	,@rcTargetTop bigint 
	,@rcTargetRight bigint 
	,@rcTargetBottom bigint 
	,@dwBitRate bigint 
	,@dwBitErrorRate bigint 
	,@AvgTimePerFrame bigint 
	,@biSize bigint 
	,@biHeight bigint 
	,@biWidth bigint 
	,@biPlanes int 
	,@biBitCount int 
	,@biCompression bigint 
	,@biSizeImage bigint 
	,@biXPelsPerMeter bigint 
	,@biYPelsPerMeter bigint 
	,@biClrUsed bigint 
	,@biClrImportant bigint 
	,@wFormatTag int 
	,@nChannels int 
	,@nSamplesPerSec bigint 
	,@nAvgBytesPerSec bigint 
	,@nBlockAlign int 
	,@wBitsPerSample int 
	,@cbSize int 
	, @isActive bit
	, @isStreamable bit
) 
AS 
INSERT INTO utPublishingPointSource 
(
	ID
	,ConferenceParticipantID
	,avSourceType
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
	,majorType
	,subType
	,fixedSizeSamples
	,temporalCompression
	,sampleSize
	,headerFormatType
	,headerFormatSize
	,rcSourceLeft
	,rcSourceTop
	,rcSourceRight
	,rcSourceBottom
	,rcTargetLeft
	,rcTargetTop
	,rcTargetRight
	,rcTargetBottom
	,dwBitRate
	,dwBitErrorRate
	,AvgTimePerFrame
	,biSize
	,biHeight
	,biWidth
	,biPlanes
	,biBitCount
	,biCompression
	,biSizeImage
	,biXPelsPerMeter
	,biYPelsPerMeter
	,biClrUsed
	,biClrImportant
	,wFormatTag
	,nChannels
	,nSamplesPerSec
	,nAvgBytesPerSec
	,nBlockAlign
	,wBitsPerSample
	,cbSize
	, isActive
	, isStreamable
) 
VALUES
(
	@ID
	,@ConferenceParticipantID
	,@avSourceType
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
	,@majorType
	,@subType
	,@fixedSizeSamples
	,@temporalCompression
	,@sampleSize
	,@headerFormatType
	,@headerFormatSize
	,@rcSourceLeft
	,@rcSourceTop
	,@rcSourceRight
	,@rcSourceBottom
	,@rcTargetLeft
	,@rcTargetTop
	,@rcTargetRight
	,@rcTargetBottom
	,@dwBitRate
	,@dwBitErrorRate
	,@AvgTimePerFrame
	,@biSize
	,@biHeight
	,@biWidth
	,@biPlanes
	,@biBitCount
	,@biCompression
	,@biSizeImage
	,@biXPelsPerMeter
	,@biYPelsPerMeter
	,@biClrUsed
	,@biClrImportant
	,@wFormatTag
	,@nChannels
	,@nSamplesPerSec
	,@nAvgBytesPerSec
	,@nBlockAlign
	,@wBitsPerSample
	,@cbSize
	, @isActive
	, @isStreamable
)
GO
