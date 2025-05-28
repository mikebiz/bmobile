/****** Object:  StoredProcedure [dbo].[uspUpdateParticipantEmail]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateParticipantEmail] (@ParticipantID uniqueidentifier ,@EmailID uniqueidentifier ,@Description nvarchar(50) ) AS UPDATE utParticipantEmail SET Description=@Description WHERE ParticipantID=@ParticipantID AND EmailID=@EmailID
GO
