/****** Object:  StoredProcedure [dbo].[uspDeleteParticipateType]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteParticipateType] (@ID uniqueidentifier ) AS DELETE FROM utParticipateType WHERE ID=@ID
GO
