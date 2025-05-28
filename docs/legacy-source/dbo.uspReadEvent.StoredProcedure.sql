/****** Object:  StoredProcedure [dbo].[uspReadEvent]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadEvent] (@ID uniqueidentifier ) AS SELECT ID,Name,Received,Sent,CorrelationID,EventType,ConferenceParticipantID,SentCounter FROM utEvent WHERE ID=@ID
GO
