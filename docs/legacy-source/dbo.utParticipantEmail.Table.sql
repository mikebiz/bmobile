/****** Object:  Table [dbo].[utParticipantEmail]    Script Date: 08/27/2008 14:35:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[utParticipantEmail](
	[ParticipantID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[EmailID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[Description] [nvarchar](50) NOT NULL DEFAULT ('ROLE'),
 CONSTRAINT [PK_utParticipantEmail] PRIMARY KEY CLUSTERED 
(
	[ParticipantID] ASC,
	[EmailID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[utParticipantEmail]  WITH CHECK ADD  CONSTRAINT [FK_utEmail_utParticipantEmail_U] FOREIGN KEY([EmailID])
REFERENCES [dbo].[utEmail] ([ID])
GO
ALTER TABLE [dbo].[utParticipantEmail] CHECK CONSTRAINT [FK_utEmail_utParticipantEmail_U]
GO
ALTER TABLE [dbo].[utParticipantEmail]  WITH CHECK ADD  CONSTRAINT [FK_utParticipant_utParticipantEmail_U] FOREIGN KEY([ParticipantID])
REFERENCES [dbo].[utParticipant] ([ID])
GO
ALTER TABLE [dbo].[utParticipantEmail] CHECK CONSTRAINT [FK_utParticipant_utParticipantEmail_U]
GO
