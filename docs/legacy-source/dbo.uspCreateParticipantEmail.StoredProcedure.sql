/****** Object:  StoredProcedure [dbo].[uspCreateParticipantEmail]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspCreateParticipantEmail] (@ParticipantID uniqueidentifier ,@EmailID uniqueidentifier ,@Description nvarchar(50) ) AS INSERT INTO utParticipantEmail (ParticipantID,EmailID,Description) VALUES (@ParticipantID,@EmailID,@Description)
GO
