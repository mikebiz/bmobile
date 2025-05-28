/****** Object:  StoredProcedure [dbo].[uspUpdateSession]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateSession] (@InUseID int ,@AvailableID int ,@Available int ) AS UPDATE utSession SET AvailableID=@AvailableID,Available=@Available WHERE InUseID=@InUseID
GO
