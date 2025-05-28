/****** Object:  StoredProcedure [dbo].[uspReadConferencef]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadConferencef] (@ConferenceType uniqueidentifier ) AS SELECT ID,Name,Description,ConferenceType,StartTime,EndTime,timeStamp FROM utConference  WHERE @ConferenceType=ConferenceType
GO
