/****** Object:  StoredProcedure [dbo].[uspCreateParticipant]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspCreateParticipant] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ) AS INSERT INTO utParticipant (ID,Name,Description) VALUES (@ID,@Name,@Description)
GO
