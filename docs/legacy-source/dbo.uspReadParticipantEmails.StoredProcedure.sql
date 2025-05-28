/****** Object:  StoredProcedure [dbo].[uspReadParticipantEmails]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspReadParticipantEmails] (@ParticipantID uniqueidentifier) AS 
SELECT PE.ParticipantID,E.ID,E.Name, E.Description 
FROM utParticipantEmail AS PE INNER JOIN utEmail AS E ON PE.EmailID=E.ID 
WHERE PE.ParticipantID=@ParticipantID
GO
