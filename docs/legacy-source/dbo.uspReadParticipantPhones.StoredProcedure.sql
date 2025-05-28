/****** Object:  StoredProcedure [dbo].[uspReadParticipantPhones]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspReadParticipantPhones] (@ParticipantID uniqueidentifier) AS 
SELECT PE.ParticipantID,E.ID,E.Name, E.Description 
FROM utParticipantPhone AS PE INNER JOIN utPhone AS E ON PE.PhoneID=E.ID 
WHERE PE.ParticipantID=@ParticipantID
GO
