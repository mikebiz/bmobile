/****** Object:  StoredProcedure [dbo].[uspUpdateParticipant]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateParticipant] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ) AS UPDATE utParticipant SET Name=@Name,Description=@Description WHERE ID=@ID
GO
