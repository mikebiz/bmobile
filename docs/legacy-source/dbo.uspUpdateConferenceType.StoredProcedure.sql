/****** Object:  StoredProcedure [dbo].[uspUpdateConferenceType]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateConferenceType] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ) AS UPDATE utConferenceType SET Name=@Name,Description=@Description WHERE ID=@ID
GO
