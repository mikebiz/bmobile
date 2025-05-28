USE [BCS]
GO
/****** Object:  StoredProcedure [dbo].[uspCreateCDR]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspCreateCDR] (@ID uniqueidentifier ,@ConferenceParticipantID uniqueidentifier ,@CDRType nvarchar(50) ,@ParticipateType nvarchar(50) ,@StartTime datetime ,@EndTime datetime ,@timeStamp datetime ,@mCostPerMin money ,@mPricePerMin money ,@mExtCost money ,@mExtPrice money ) AS 
DECLARE @CDRTypeID uniqueidentifier ,@ParticipateTypeID uniqueidentifier 
SELECT @CDRTypeID=ID FROM utCDRType WHERE Name=@CDRType
SELECT @ParticipateTypeID=ID FROM utParticipateType WHERE Name=@ParticipateType
INSERT INTO utCDR (ID,ConferenceParticipantID,CDRType,ParticipateType,StartTime,EndTime,timeStamp,mCostPerMin,mPricePerMin,mExtCost,mExtPrice) VALUES (@ID,@ConferenceParticipantID,@CDRTypeID,@ParticipateTypeID,@StartTime,@EndTime,@timeStamp,@mCostPerMin,@mPricePerMin,@mExtCost,@mExtPrice)
GO
