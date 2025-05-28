USE [BCS]
GO
/****** Object:  UserDefinedFunction [dbo].[NextAvailable]    Script Date: 08/27/2008 14:37:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date, ,>
-- Description:	<Description, ,>
-- =============================================
CREATE FUNCTION [dbo].[NextAvailable] ()
RETURNS int
AS
BEGIN
	-- Declare the return variable here
	DECLARE @nextAvailable int

SELECT @nextAvailable=MIN (AvailableID) FROM utSession WHERE Available=1

	-- Return the result of the function
	RETURN @nextAvailable

END
GO
