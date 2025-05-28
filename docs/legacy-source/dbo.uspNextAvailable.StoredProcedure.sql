/****** Object:  StoredProcedure [dbo].[uspNextAvailable]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date, ,>
-- Description:	<Description, ,>
-- =============================================
CREATE PROCEDURE [dbo].[uspNextAvailable] (@nextAvailable int out)
AS
BEGIN
-- Declare the return variable here
DECLARE @newAvailable int
SELECT @nextAvailable=MIN (AvailableID) FROM utSession WHERE Available=1

SET @newAvailable=1
IF EXISTS (SELECT InUseID FROM utSession WHERE InUseID=@nextAvailable+1)
BEGIN
	SET @newAvailable=0
END


UPDATE utSession SET Available=0 WHERE AvailableID=@nextAvailable
INSERT INTO utSession (InUseID, AvailableID, Available) VALUES (@nextAvailable, @nextAvailable+1, @newAvailable)

END
GO
