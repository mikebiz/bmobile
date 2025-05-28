/****** Object:  StoredProcedure [dbo].[uspCreateServer]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspCreateServer] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ,@ManagementQueue nvarchar(100) ,@ServerType int ) AS INSERT INTO utServer (ID,Name,Description,ManagementQueue,ServerType) VALUES (@ID,@Name,@Description,@ManagementQueue,@ServerType)
GO
