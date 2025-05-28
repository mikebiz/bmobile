/****** Object:  StoredProcedure [dbo].[uspUpdatePublishingPointSource]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspUpdatePublishingPointSource] 
(
	@ID uniqueidentifier 
	, @ConferenceParticipantID uniqueidentifier 
	, @avSourceType int
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
	, @majorType uniqueidentifier 
	, @subType uniqueidentifier 
	, @fixedSizeSamples int 
	, @temporalCompression int 
	, @sampleSize int 
	, @headerFormatType uniqueidentifier 
	, @headerFormatSize bigint 
	, @rcSourceLeft bigint 
	, @rcSourceTop bigint 
	, @rcSourceRight bigint 
	, @rcSourceBottom bigint 
	, @rcTargetLeft bigint 
	, @rcTargetTop bigint 
	, @rcTargetRight bigint 
	, @rcTargetBottom bigint 
	, @dwBitRate bigint 
	, @dwBitErrorRate bigint 
	, @AvgTimePerFrame bigint 
	, @biSize bigint 
	, @biHeight bigint 
	, @biWidth bigint 
	, @biPlanes int 
	, @biBitCount int 
	, @biCompression bigint 
	, @biSizeImage bigint 
	, @biXPelsPerMeter bigint 
	, @biYPelsPerMeter bigint 
	, @biClrUsed bigint 
	, @biClrImportant bigint 
	, @wFormatTag int 
	, @nChannels int 
	, @nSamplesPerSec bigint 
	, @nAvgBytesPerSec bigint 
	, @nBlockAlign int 
	, @wBitsPerSample int 
	, @cbSize int 
	, @isActive bit
	, @isStreamable bit
) 
AS 

UPDATE utPublishingPointSource 
SET avSourceType = @avSourceType
	, ipVerLen = @ipVerLen
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
	, majorType=@majorType
	, subType=@subType
	, fixedSizeSamples=@fixedSizeSamples
	, temporalCompression=@temporalCompression
	, sampleSize=@sampleSize
	, headerFormatType=@headerFormatType
	, headerFormatSize=@headerFormatSize
	, rcSourceLeft=@rcSourceLeft
	, rcSourceTop=@rcSourceTop
	, rcSourceRight=@rcSourceRight
	, rcSourceBottom=@rcSourceBottom
	, rcTargetLeft=@rcTargetLeft
	, rcTargetTop=@rcTargetTop
	, rcTargetRight=@rcTargetRight
	, rcTargetBottom=@rcTargetBottom
	, dwBitRate=@dwBitRate
	, dwBitErrorRate=@dwBitErrorRate
	, AvgTimePerFrame=@AvgTimePerFrame
	, biSize=@biSize
	, biHeight=@biHeight
	, biWidth=@biWidth
	, biPlanes=@biPlanes
	, biBitCount=@biBitCount
	, biCompression=@biCompression
	, biSizeImage=@biSizeImage
	, biXPelsPerMeter=@biXPelsPerMeter
	, biYPelsPerMeter=@biYPelsPerMeter
	, biClrUsed=@biClrUsed
	, biClrImportant=@biClrImportant
	, wFormatTag=@wFormatTag
	, nChannels=@nChannels
	, nSamplesPerSec=@nSamplesPerSec
	, nAvgBytesPerSec=@nAvgBytesPerSec
	, nBlockAlign=@nBlockAlign
	, wBitsPerSample=@wBitsPerSample
	, cbSize=@cbSize
	, isActive = @isActive
	, isStreamable = @isStreamable
	, ConferenceParticipantID=@ConferenceParticipantID 
WHERE ID=@ID
GO
