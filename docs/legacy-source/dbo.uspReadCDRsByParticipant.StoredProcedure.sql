/****** Object:  StoredProcedure [dbo].[uspReadCDRsByParticipant]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspReadCDRsByParticipant] (@ParticipantID uniqueidentifier) AS 
SELECT C.ID,C.ConferenceParticipantID,CT.Name AS CDRType,PT.Name AS ParticipateType,C.StartTime,C.EndTime,C.timeStamp,C.mCostPerMin,C.mPricePerMin,C.mExtCost,C.mExtPrice 
FROM utCDR AS C
INNER JOIN utConferenceParticipant AS CP ON C.ConferenceParticipantID=CP.ID
INNER JOIN utCDRType AS CT ON C.CDRType=CT.ID
INNER JOIN utParticipateType AS PT on C.ParticipateType=PT.ID
WHERE CP.ParticipantID=@ParticipantID
GO
