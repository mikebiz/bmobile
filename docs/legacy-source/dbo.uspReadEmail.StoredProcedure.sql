/****** Object:  StoredProcedure [dbo].[uspReadEmail]    Script Date: 08/27/2008 14:36:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadEmail] (@ID uniqueidentifier ) AS SELECT ID,Name,Description FROM utEmail WHERE ID=@ID
GO
