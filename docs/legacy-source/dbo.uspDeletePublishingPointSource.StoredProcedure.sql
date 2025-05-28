/****** Object:  StoredProcedure [dbo].[uspDeletePublishingPointSource]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeletePublishingPointSource] (@ID uniqueidentifier ) AS DELETE FROM utPublishingPointSource WHERE ID=@ID
GO
