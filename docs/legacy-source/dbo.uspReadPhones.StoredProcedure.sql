/****** Object:  StoredProcedure [dbo].[uspReadPhones]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadPhones] AS SELECT ID,Name,Description FROM utPhone
GO
