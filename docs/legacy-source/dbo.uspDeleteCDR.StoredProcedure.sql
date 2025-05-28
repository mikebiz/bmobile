/****** Object:  StoredProcedure [dbo].[uspDeleteCDR]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteCDR] (@ID uniqueidentifier ) AS DELETE FROM utCDR WHERE ID=@ID
GO
