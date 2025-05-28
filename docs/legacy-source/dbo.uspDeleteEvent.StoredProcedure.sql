/****** Object:  StoredProcedure [dbo].[uspDeleteEvent]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteEvent] (@ID uniqueidentifier ) AS DELETE FROM utEvent WHERE ID=@ID
GO
