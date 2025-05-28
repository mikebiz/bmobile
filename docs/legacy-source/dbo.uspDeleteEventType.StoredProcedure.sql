/****** Object:  StoredProcedure [dbo].[uspDeleteEventType]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteEventType] (@ID int ) AS DELETE FROM utEventType WHERE ID=@ID
GO
