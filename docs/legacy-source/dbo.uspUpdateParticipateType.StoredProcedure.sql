/****** Object:  StoredProcedure [dbo].[uspUpdateParticipateType]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateParticipateType] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ) AS UPDATE utParticipateType SET Name=@Name,Description=@Description WHERE ID=@ID
GO
