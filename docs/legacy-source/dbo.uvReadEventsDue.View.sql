USE [BCS]
GO
/****** Object:  View [dbo].[uvReadEventsDue]    Script Date: 08/27/2008 14:46:10 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE View [dbo].[uvReadEventsDue] AS 

SELECT E.ID,E.Name,E.Received,E.Sent,E.CorrelationID,E.EventType,E.ConferenceParticipantID 
FROM utEvent E 
INNER JOIN utEventType ET ON E.EventType=ET.ID
INNER JOIN utConferenceParticipant CP ON E.ConferenceParticipantID=CP.ID
WHERE (E.Received < '01/01/2100' AND DATEDIFF (ms, GETDATE(), E.Received) >= ET.Threshold) OR
(E.Received >= '01/01/2100' AND E.SentCounter < ET.Retry)
GO
