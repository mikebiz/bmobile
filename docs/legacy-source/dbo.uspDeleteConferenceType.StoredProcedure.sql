/****** Object:  StoredProcedure [dbo].[uspDeleteConferenceType]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteConferenceType] (@ID uniqueidentifier ) AS DELETE FROM utConferenceType WHERE ID=@ID
GO
