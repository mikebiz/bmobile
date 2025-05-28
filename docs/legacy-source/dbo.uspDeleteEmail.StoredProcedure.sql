/****** Object:  StoredProcedure [dbo].[uspDeleteEmail]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspDeleteEmail] (@ID uniqueidentifier ) AS 
DELETE FROM utParticipantEmail WHERE EmailID=@ID
DELETE FROM utEmail WHERE ID=@ID
GO
