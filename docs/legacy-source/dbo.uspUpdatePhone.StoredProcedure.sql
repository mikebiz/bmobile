/****** Object:  StoredProcedure [dbo].[uspUpdatePhone]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdatePhone] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ) AS UPDATE utPhone SET Description=@Description,Name=@Name WHERE ID=@ID
GO
