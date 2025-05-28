/****** Object:  StoredProcedure [dbo].[uspUpdateServer]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateServer] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ,@ManagementQueue nvarchar(100) ,@ServerType int ) AS UPDATE utServer SET Name=@Name,Description=@Description,ManagementQueue=@ManagementQueue,ServerType=@ServerType WHERE ID=@ID
GO
