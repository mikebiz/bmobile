/****** Object:  StoredProcedure [dbo].[uspCreateEmail]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspCreateEmail] (@ParticipantID uniqueidentifier ,@EmailID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ) AS 
INSERT INTO utEmail (ID,Name,Description) VALUES (@EmailID,@Name,@Description)
EXEC dbo.uspCreateParticipantEmail @ParticipantID, @EmailID, @Description;
GO
