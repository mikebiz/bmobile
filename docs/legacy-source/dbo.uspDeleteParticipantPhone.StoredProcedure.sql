/****** Object:  StoredProcedure [dbo].[uspDeleteParticipantPhone]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteParticipantPhone] (@ParticipantID uniqueidentifier ,@PhoneID uniqueidentifier ) AS DELETE FROM utParticipantPhone WHERE ParticipantID=@ParticipantID AND PhoneID=@PhoneID
GO
