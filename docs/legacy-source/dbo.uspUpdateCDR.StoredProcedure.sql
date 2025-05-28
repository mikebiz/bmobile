/****** Object:  StoredProcedure [dbo].[uspUpdateCDR]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspUpdateCDR] (@ID uniqueidentifier ,@ConferenceParticipantID uniqueidentifier ,@CDRType nvarchar(50) ,@ParticipateType nvarchar(50) ,@StartTime datetime ,@EndTime datetime ,@timeStamp datetime ,@mCostPerMin money ,@mPricePerMin money ,@mExtCost money ,@mExtPrice money ) AS 
DECLARE @CDRTypeID uniqueidentifier ,@ParticipateTypeID uniqueidentifier 
SELECT @CDRTypeID=ID FROM utCDRType WHERE Name=@CDRType
SELECT @ParticipateTypeID=ID FROM utParticipateType WHERE Name=@ParticipateType
UPDATE utCDR SET StartTime=@StartTime,EndTime=@EndTime,timeStamp=@timeStamp,mCostPerMin=@mCostPerMin,mPricePerMin=@mPricePerMin,mExtCost=@mExtCost,mExtPrice=@mExtPrice,CDRType=@CDRTypeID,ParticipateType=@ParticipateTypeID WHERE ID=@ID
GO
