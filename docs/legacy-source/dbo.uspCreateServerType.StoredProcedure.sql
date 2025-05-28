/****** Object:  StoredProcedure [dbo].[uspCreateServerType]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspCreateServerType] (@ID int ,@Name nvarchar(50) ) AS INSERT INTO utServerType (ID,Name) VALUES (@ID,@Name)
GO
