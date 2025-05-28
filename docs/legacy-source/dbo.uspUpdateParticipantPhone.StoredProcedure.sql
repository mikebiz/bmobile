/****** Object:  StoredProcedure [dbo].[uspUpdateParticipantPhone]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateParticipantPhone] (@ParticipantID uniqueidentifier ,@PhoneID uniqueidentifier ,@Description nvarchar(50) ) AS UPDATE utParticipantPhone SET Description=@Description WHERE ParticipantID=@ParticipantID AND PhoneID=@PhoneID
GO
