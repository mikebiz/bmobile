/****** Object:  StoredProcedure [dbo].[uspReadPublishingPointSource]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadPublishingPointSource] (@ID uniqueidentifier ) AS SELECT ID,ConferenceParticipantID,srcIP,destIP,srcPort,destPort,majorType,subType,fixedSizeSamples,temporalCompression,sampleSize,headerFormatType,headerFormatSize,rcSourceLeft,rcSourceTop,rcSourceRight,rcSourceBottom,rcTargetLeft,rcTargetTop,rcTargetRight,rcTargetBottom,dwBitRate,dwBitErrorRate,AvgTimePerFrame,biSize,biHeight,biWidth,biPlanes,biBitCount,biCompression,biSizeImage,biXPelsPerMeter,biYPelsPerMeter,biClrUsed,biClrImportant,wFormatTag,nChannels,nSamplesPerSec,nAvgBytesPerSec,nBlockAlign,wBitsPerSample,cbSize,lastUpdated FROM utPublishingPointSource WHERE ID=@ID
GO
