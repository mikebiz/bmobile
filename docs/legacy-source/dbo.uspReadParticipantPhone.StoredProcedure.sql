/****** Object:  StoredProcedure [dbo].[uspReadParticipantPhone]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadParticipantPhone] (@ParticipantID uniqueidentifier ,@PhoneID uniqueidentifier ) AS SELECT ParticipantID,PhoneID,Description FROM utParticipantPhone WHERE ParticipantID=@ParticipantID AND PhoneID=@PhoneID
GO
