/****** Object:  StoredProcedure [dbo].[uspReadParticipantPhonef]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadParticipantPhonef]  AS SELECT ParticipantID,PhoneID,Description FROM utParticipantPhone
GO
