/****** Object:  StoredProcedure [dbo].[uspUpdateServerType]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateServerType] (@ID int ,@Name nvarchar(50) ) AS UPDATE utServerType SET Name=@Name WHERE ID=@ID
GO
