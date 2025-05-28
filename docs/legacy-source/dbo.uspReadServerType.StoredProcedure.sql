/****** Object:  StoredProcedure [dbo].[uspReadServerType]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadServerType] (@ID int ) AS SELECT ID,Name FROM utServerType WHERE ID=@ID
GO
