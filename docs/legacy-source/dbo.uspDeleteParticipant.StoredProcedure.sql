/****** Object:  StoredProcedure [dbo].[uspDeleteParticipant]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteParticipant] (@ID uniqueidentifier ) AS DELETE FROM utParticipant WHERE ID=@ID
GO
