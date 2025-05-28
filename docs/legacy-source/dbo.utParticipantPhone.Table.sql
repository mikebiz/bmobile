/****** Object:  Table [dbo].[utParticipantPhone]    Script Date: 08/27/2008 14:35:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[utParticipantPhone](
	[ParticipantID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[PhoneID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[Description] [nvarchar](50) NOT NULL DEFAULT ('ROLE'),
 CONSTRAINT [PK_utParticipantPhone] PRIMARY KEY CLUSTERED 
(
	[ParticipantID] ASC,
	[PhoneID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[utParticipantPhone]  WITH CHECK ADD  CONSTRAINT [FK_utParticipant_utParticipantPhone_U] FOREIGN KEY([ParticipantID])
REFERENCES [dbo].[utParticipant] ([ID])
GO
ALTER TABLE [dbo].[utParticipantPhone] CHECK CONSTRAINT [FK_utParticipant_utParticipantPhone_U]
GO
ALTER TABLE [dbo].[utParticipantPhone]  WITH CHECK ADD  CONSTRAINT [FK_utPhone_utParticipantPhone_U] FOREIGN KEY([PhoneID])
REFERENCES [dbo].[utPhone] ([ID])
GO
ALTER TABLE [dbo].[utParticipantPhone] CHECK CONSTRAINT [FK_utPhone_utParticipantPhone_U]
GO
