/****** Object:  StoredProcedure [dbo].[uspCreatePhone]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspCreatePhone] (@ParticipantID uniqueidentifier, @PhoneID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ) AS 
INSERT INTO utPhone (ID,Name,Description) VALUES (@PhoneID,@Name,@Description)
EXEC dbo.uspCreateParticipantPhone @ParticipantID, @PhoneID, @Description;
GO
