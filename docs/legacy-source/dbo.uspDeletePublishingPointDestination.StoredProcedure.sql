/****** Object:  StoredProcedure [dbo].[uspDeletePublishingPointDestination]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeletePublishingPointDestination] (@ID uniqueidentifier ) AS DELETE FROM utPublishingPointDestination WHERE ID=@ID
GO
