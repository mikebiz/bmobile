/****** Object:  StoredProcedure [dbo].[uspDeleteParticipantType]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteParticipantType] (@ID uniqueidentifier ) AS DELETE FROM utParticipantType WHERE ID=@ID
GO
