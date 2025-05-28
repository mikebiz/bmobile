/****** Object:  StoredProcedure [dbo].[uspUpdateEvent]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateEvent] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Received datetime ,@Sent datetime ,@CorrelationID uniqueidentifier ,@EventType int ,@ConferenceParticipantID uniqueidentifier ) AS UPDATE utEvent SET Name=@Name,Received=@Received,Sent=@Sent,CorrelationID=@CorrelationID,EventType=@EventType,ConferenceParticipantID=@ConferenceParticipantID WHERE ID=@ID
GO
