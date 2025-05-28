/****** Object:  StoredProcedure [dbo].[uspUpdateCDRType]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateCDRType] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ) AS UPDATE utCDRType SET Name=@Name,Description=@Description WHERE ID=@ID
GO
