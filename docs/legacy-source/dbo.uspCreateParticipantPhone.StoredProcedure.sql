/****** Object:  StoredProcedure [dbo].[uspCreateParticipantPhone]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspCreateParticipantPhone] (@ParticipantID uniqueidentifier ,@PhoneID uniqueidentifier ,@Description nvarchar(50) ) AS INSERT INTO utParticipantPhone (ParticipantID,PhoneID,Description) VALUES (@ParticipantID,@PhoneID,@Description)
GO
