/****** Object:  StoredProcedure [dbo].[uspCreateConferenceParticipant]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspCreateConferenceParticipant] (@ID uniqueidentifier ,@ConferenceID uniqueidentifier ,@ParticipantID uniqueidentifier ,@ParticipantType nvarchar(50) ,@Description nvarchar(50), @MacAddress nvarchar(50), @isActive int, @Mask int ) AS 
DECLARE @ParticipantTypeID uniqueidentifier
SELECT @ParticipantTypeID=ID FROM utParticipantType WHERE Name=@ParticipantType
INSERT INTO utConferenceParticipant (ID,ConferenceID,ParticipantID,ParticipantType,Description,MacAddress,isActive,Mask) VALUES (@ID,@ConferenceID,@ParticipantID,@ParticipantTypeID,@Description,@MacAddress,@isActive,@Mask)
GO
