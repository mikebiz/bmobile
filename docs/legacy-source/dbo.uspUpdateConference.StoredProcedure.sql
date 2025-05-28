/****** Object:  StoredProcedure [dbo].[uspUpdateConference]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspUpdateConference] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ,@ConferenceType nvarchar(50),@StartTime datetime ,@EndTime datetime ,@timeStamp datetime ,@isActive integer ,@LongDescription nvarchar(1024)) AS 
DECLARE @ConferenceTypeID uniqueidentifier
SELECT @ConferenceTypeID=ID FROM utConferenceType WHERE Name=@ConferenceType
UPDATE utConference SET Name=@Name,Description=@Description,StartTime=@StartTime,EndTime=@EndTime,timeStamp=@timeStamp,ConferenceType=@ConferenceTypeID,isActive=@isActive,LongDescription=@LongDescription WHERE ID=@ID
GO
