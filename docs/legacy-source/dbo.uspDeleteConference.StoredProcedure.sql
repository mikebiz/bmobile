/****** Object:  StoredProcedure [dbo].[uspDeleteConference]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspDeleteConference] (@ID uniqueidentifier ) AS 
DECLARE @SessionID int
SELECT @SessionID FROM utConference WHERE ID=@ID
EXEC uspResetSession @SessionID
DELETE FROM utConference WHERE ID=@ID
GO
