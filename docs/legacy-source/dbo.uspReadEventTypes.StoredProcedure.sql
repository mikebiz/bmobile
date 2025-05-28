/****** Object:  StoredProcedure [dbo].[uspReadEventTypes]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadEventTypes] AS SELECT ID,Name,Threshold,Retry FROM utEventType
GO
