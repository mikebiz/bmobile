/****** Object:  StoredProcedure [dbo].[uspCreateEvent]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspCreateEvent] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Received datetime ,@Sent datetime ,@CorrelationID uniqueidentifier ,@EventType int ,@ConferenceParticipantID uniqueidentifier ) AS INSERT INTO utEvent (ID,Name,Received,Sent,CorrelationID,EventType,ConferenceParticipantID) VALUES (@ID,@Name,@Received,@Sent,@CorrelationID,@EventType,@ConferenceParticipantID)
GO
