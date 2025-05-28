/****** Object:  StoredProcedure [dbo].[uspDeleteSession]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteSession] (@InUseID int ) AS DELETE FROM utSession WHERE InUseID=@InUseID
GO
