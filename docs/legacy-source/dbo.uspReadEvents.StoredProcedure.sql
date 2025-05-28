/****** Object:  StoredProcedure [dbo].[uspReadEvents]    Script Date: 08/27/2008 14:42:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadEvents] AS SELECT ID,Name,Received,Sent,CorrelationID,EventType,ConferenceParticipantID,SentCounter FROM utEvent
GO
