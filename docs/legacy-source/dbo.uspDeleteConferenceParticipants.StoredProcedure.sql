/****** Object:  StoredProcedure [dbo].[uspDeleteConferenceParticipants]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspDeleteConferenceParticipants] (@ConferenceID uniqueidentifier ) AS 
IF NOT EXISTS (SELECT CDR.ID FROM utCDR AS CDR INNER JOIN utConferenceParticipant AS CP ON CDR.ConferenceParticipantID=CP.ID WHERE ConferenceID=@ConferenceID)
BEGIN
DELETE FROM utConferenceParticipant WHERE ConferenceID=@ConferenceID
END
GO
