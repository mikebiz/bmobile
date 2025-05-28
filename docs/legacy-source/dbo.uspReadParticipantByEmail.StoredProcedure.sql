/****** Object:  StoredProcedure [dbo].[uspReadParticipantByEmail]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspReadParticipantByEmail] (@Email nvarchar(50) ) AS 
SELECT P.ID,P.Name,P.Description FROM utParticipant AS P
INNER JOIN utParticipantEmail AS PE ON P.ID=PE.ParticipantID
INNER JOIN utEmail AS E ON PE.EmailID=E.ID
WHERE E.Name=@Email
GO
