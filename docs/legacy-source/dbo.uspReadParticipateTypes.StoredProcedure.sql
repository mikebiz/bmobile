/****** Object:  StoredProcedure [dbo].[uspReadParticipateTypes]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadParticipateTypes] AS SELECT ID,Name,Description FROM utParticipateType
GO
