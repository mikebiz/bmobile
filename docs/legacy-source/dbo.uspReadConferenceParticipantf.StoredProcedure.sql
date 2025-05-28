/****** Object:  StoredProcedure [dbo].[uspReadConferenceParticipantf]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspReadConferenceParticipantf] (@ConferenceID uniqueidentifier ,@ParticipantID uniqueidentifier ,@ParticipantType uniqueidentifier ) AS 
SELECT ID,ConferenceID,ParticipantID,ParticipantType,Description,MacAddress,isActive,Mask  FROM utConferenceParticipant  WHERE @ConferenceID=ConferenceID AND @ParticipantID=ParticipantID AND @ParticipantType=ParticipantType
GO
