/****** Object:  StoredProcedure [dbo].[uspUpdateEmail]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateEmail] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ) AS UPDATE utEmail SET Description=@Description,Name=@Name WHERE ID=@ID
GO
