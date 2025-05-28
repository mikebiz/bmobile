/****** Object:  StoredProcedure [dbo].[uspDeleteCDRType]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteCDRType] (@ID uniqueidentifier ) AS DELETE FROM utCDRType WHERE ID=@ID
GO
