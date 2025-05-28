/****** Object:  StoredProcedure [dbo].[uspResetSession]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date, ,>
-- Description:	<Description, ,>
-- =============================================
CREATE PROCEDURE [dbo].[uspResetSession] (@inUseID integer)
AS
BEGIN
-- Declare the return variable here
DELETE FROM utSession WHERE inUseID=@inUseID
UPDATE utSession SET Available=1 WHERE AvailableID=@inUseID

END
GO
