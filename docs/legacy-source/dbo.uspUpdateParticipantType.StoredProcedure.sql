/****** Object:  StoredProcedure [dbo].[uspUpdateParticipantType]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateParticipantType] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ) AS UPDATE utParticipantType SET Name=@Name,Description=@Description WHERE ID=@ID
GO
