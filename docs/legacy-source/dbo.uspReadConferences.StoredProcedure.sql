/****** Object:  StoredProcedure [dbo].[uspReadConferences]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspReadConferences] AS SELECT ID,Name,Description,ConferenceType,StartTime,EndTime,timeStamp,isActive,LongDescription,SessionID FROM utConference
GO
