/****** Object:  Table [dbo].[utParticipant]    Script Date: 08/27/2008 14:35:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[utParticipant](
	[ID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[Name] [nvarchar](50) NOT NULL DEFAULT ('Name'),
	[Description] [nvarchar](50) NOT NULL DEFAULT ('ROLE'),
 CONSTRAINT [PK_utParticipant] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
