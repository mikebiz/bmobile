/****** Object:  StoredProcedure [dbo].[uspDeleteServerType]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteServerType] (@ID int ) AS DELETE FROM utServerType WHERE ID=@ID
GO
