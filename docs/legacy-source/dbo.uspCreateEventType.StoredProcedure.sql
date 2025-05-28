/****** Object:  StoredProcedure [dbo].[uspCreateEventType]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create procedure [dbo].[uspCreateEventType] (@ID int ,@Name nvarchar(50) ,@Threshold int ,@Retry int ) AS INSERT INTO utEventType (ID,Name,Threshold,Retry) VALUES (@ID,@Name,@Threshold,@Retry)
GO
