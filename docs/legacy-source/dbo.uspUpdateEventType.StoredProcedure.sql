/****** Object:  StoredProcedure [dbo].[uspUpdateEventType]    Script Date: 08/27/2008 14:36:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspUpdateEventType] (@ID int ,@Name nvarchar(50) ,@Threshold int ,@Retry int ) AS UPDATE utEventType SET Name=@Name,Threshold=@Threshold,Retry=@Retry WHERE ID=@ID
GO
