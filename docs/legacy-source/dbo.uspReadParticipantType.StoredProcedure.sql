/****** Object:  StoredProcedure [dbo].[uspReadParticipantType]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadParticipantType] (@ID uniqueidentifier ) AS SELECT ID,Name,Description FROM utParticipantType WHERE ID=@ID
GO
