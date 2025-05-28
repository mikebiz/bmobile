/****** Object:  StoredProcedure [dbo].[uspDeletePhone]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspDeletePhone] (@ID uniqueidentifier ) AS 
DELETE FROM utParticipantPhone WHERE PhoneID=@ID
DELETE FROM utPhone WHERE ID=@ID
GO
