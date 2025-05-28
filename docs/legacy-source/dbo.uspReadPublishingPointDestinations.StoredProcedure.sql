/****** Object:  StoredProcedure [dbo].[uspReadPublishingPointDestinations]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadPublishingPointDestinations] AS SELECT ID,ConferenceParticipantID,srcIP,destIP,srcPort,destPort FROM utPublishingPointDestination
GO
