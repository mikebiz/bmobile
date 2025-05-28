/****** Object:  StoredProcedure [dbo].[uspReadCDRs]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadCDRs] AS SELECT ID,ConferenceParticipantID,CDRType,ParticipateType,StartTime,EndTime,timeStamp,mCostPerMin,mPricePerMin,mExtCost,mExtPrice FROM utCDR
GO
