/****** Object:  StoredProcedure [dbo].[uspReadPublishingPointDestinationf]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadPublishingPointDestinationf] (@ConferenceParticipantID uniqueidentifier ) AS SELECT ID,ConferenceParticipantID,srcIP,destIP,srcPort,destPort FROM utPublishingPointDestination  WHERE @ConferenceParticipantID=ConferenceParticipantID
GO
