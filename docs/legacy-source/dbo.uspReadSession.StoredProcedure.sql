/****** Object:  StoredProcedure [dbo].[uspReadSession]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadSession] (@InUseID int ) AS SELECT InUseID,AvailableID,Available FROM utSession WHERE InUseID=@InUseID
GO
