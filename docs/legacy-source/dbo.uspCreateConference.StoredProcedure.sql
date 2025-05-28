/****** Object:  StoredProcedure [dbo].[uspCreateConference]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspCreateConference] (@ID uniqueidentifier ,@Name nvarchar(50) ,@Description nvarchar(50) ,@ConferenceType nvarchar(50) ,@StartTime datetime ,@EndTime datetime ,@timeStamp datetime, @isActive integer, @LongDescription nvarchar(1024)) AS 
DECLARE @ConferenceTypeID uniqueidentifier, @SessionID integer
SELECT @ConferenceTypeID=ID FROM utConferenceType WHERE Name=@ConferenceType
EXEC uspNextAvailable @nextAvailable = @SessionID out;
INSERT INTO utConference (ID,Name,Description,ConferenceType,StartTime,EndTime,timeStamp,isActive,LongDescription,SessionID) 
VALUES (@ID,@Name,@Description,@ConferenceTypeID,@StartTime,@EndTime,@timeStamp,@isActive,@LongDescription,@SessionID)
SELECT ID,Name,Description,ConferenceType,StartTime,EndTime,timeStamp,isActive,LongDescription,SessionID FROM utConference WHERE ID=@ID
GO
