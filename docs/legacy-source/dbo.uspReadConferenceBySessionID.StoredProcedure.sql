/****** Object:  StoredProcedure [dbo].[uspReadConferenceBySessionID]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspReadConferenceBySessionID] (@SessionID int) AS 
SELECT ID,Name,Description,ConferenceType,StartTime,EndTime,timeStamp,isActive,LongDescription,SessionID FROM utConference  WHERE SessionID=@SessionID
GO
