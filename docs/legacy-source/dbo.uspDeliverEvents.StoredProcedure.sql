/****** Object:  StoredProcedure [dbo].[uspDeliverEvents]    Script Date: 08/27/2008 14:42:19 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspDeliverEvents] AS 

DECLARE @ID uniqueidentifier ,@Name nvarchar(50) ,@Received datetime ,@Sent datetime ,@CorrelationID uniqueidentifier ,@EventType int ,@ConferenceParticipantID uniqueidentifier ,@SentCounter int 
DECLARE @destIP int, @destPort int, @keepAlive int

DECLARE EventCursor CURSOR FOR 
SELECT E.ID,E.Sent,E.SentCounter,PPD.destIP,PPD.destPort
FROM utEvent E INNER JOIN utPublischingPortDestination PPD ON PPD.ConferenceParticipantID=E.ConferenceParticipantID
WHERE E.Sent >= '01/01/2100'

OPEN EventCursor

FETCH NEXT FROM EventCursor
INTO @ID,@Sent,@SentCounter,@destIP,@destPort

WHILE @@FETCH_STATUS = 0
BEGIN

	UPDATE utEvent SET Sent=GETDATE(), SentCounter=@SentCounter+1 WHERE ID=@ID

	--EXEC xp_blx_DeliverEvent @destIP, @destPort, 

	FETCH NEXT FROM EventCursor
	INTO @ID,@Sent,@SentCounter,@destIP,@destPort,@keepAlive

END 

CLOSE EventCursor
DEALLOCATE EventCursor
GO
