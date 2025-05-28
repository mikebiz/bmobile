/****** Object:  StoredProcedure [dbo].[uspShipEvents]    Script Date: 08/27/2008 14:42:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspShipEvents] AS 
UPDATE utEvent SET Sent = '01/01/2100' WHERE ID IN (SELECT ID FROM uvReadEventsDue)
GO
