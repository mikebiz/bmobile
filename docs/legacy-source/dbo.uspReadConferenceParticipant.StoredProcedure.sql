/****** Object:  StoredProcedure [dbo].[uspReadConferenceParticipant]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspReadConferenceParticipant] (@ID uniqueidentifier ) AS 
SELECT CP.ID,CP.ConferenceID,CP.ParticipantID,PT.Name AS ParticipantType,CP.Description,CP.MacAddress,CP.isActive,CP.Mask  
FROM utConferenceParticipant AS CP INNER JOIN utParticipantType AS PT ON CP.ParticipantType=PT.ID 
WHERE CP.ID=@ID
GO
