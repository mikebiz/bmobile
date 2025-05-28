/****** Object:  StoredProcedure [dbo].[uspDeleteServer]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteServer] (@ID uniqueidentifier ) AS DELETE FROM utServer WHERE ID=@ID
GO
