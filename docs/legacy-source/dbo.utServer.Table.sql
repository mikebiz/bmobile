/****** Object:  Table [dbo].[utServer]    Script Date: 08/27/2008 14:35:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[utServer](
	[ID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[Name] [nvarchar](50) NOT NULL DEFAULT ('Name'),
	[Description] [nvarchar](50) NOT NULL DEFAULT ('ROLE'),
	[ManagementQueue] [nvarchar](100) NOT NULL DEFAULT ('ROLE'),
	[ServerType] [int] NOT NULL DEFAULT ((0)),
 CONSTRAINT [PK_utServer] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[utServer]  WITH CHECK ADD  CONSTRAINT [FK_utServerType_utServer_U] FOREIGN KEY([ServerType])
REFERENCES [dbo].[utServerType] ([ID])
GO
ALTER TABLE [dbo].[utServer] CHECK CONSTRAINT [FK_utServerType_utServer_U]
GO
