/****** Object:  StoredProcedure [dbo].[uspReadCDRType]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadCDRType] (@ID uniqueidentifier ) AS SELECT ID,Name,Description FROM utCDRType WHERE ID=@ID
GO
