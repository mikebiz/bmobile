/****** Object:  StoredProcedure [dbo].[uspReadServers]    Script Date: 08/27/2008 14:36:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspReadServers] AS SELECT ID,Name,Description,ManagementQueue,ServerType FROM utServer
GO
