/****** Object:  StoredProcedure [dbo].[uspDeleteParticipantEmail]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspDeleteParticipantEmail] (@ParticipantID uniqueidentifier ,@EmailID uniqueidentifier ) AS DELETE FROM utParticipantEmail WHERE ParticipantID=@ParticipantID AND EmailID=@EmailID
GO
