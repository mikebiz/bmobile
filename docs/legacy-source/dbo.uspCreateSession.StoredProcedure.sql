/****** Object:  StoredProcedure [dbo].[uspCreateSession]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspCreateSession] (@InUseID int ,@AvailableID int ,@Available int ) AS INSERT INTO utSession (InUseID,AvailableID,Available) VALUES (@InUseID,@AvailableID,@Available)
GO
