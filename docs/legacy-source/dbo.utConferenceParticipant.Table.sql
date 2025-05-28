/****** Object:  Table [dbo].[utConferenceParticipant]    Script Date: 08/27/2008 14:35:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[utConferenceParticipant](
	[ID] [uniqueidentifier] NOT NULL CONSTRAINT [DF__utConference__ID__7325FEFD]  DEFAULT (newid()),
	[ConferenceID] [uniqueidentifier] NOT NULL CONSTRAINT [DF__utConfere__Confe__741A2336]  DEFAULT (newid()),
	[ParticipantID] [uniqueidentifier] NOT NULL CONSTRAINT [DF__utConfere__Parti__750E476F]  DEFAULT (newid()),
	[ParticipantType] [uniqueidentifier] NOT NULL CONSTRAINT [DF__utConfere__Parti__76026BA8]  DEFAULT (newid()),
	[Description] [nvarchar](50) NOT NULL CONSTRAINT [DF__utConfere__Descr__76F68FE1]  DEFAULT ('ROLE'),
	[MacAddress] [char](24) NOT NULL,
	[isActive] [int] NOT NULL,
	[Mask] [int] NOT NULL,
 CONSTRAINT [PK_utConferenceParticipant] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY],
 CONSTRAINT [IX_utConferenceParticipant_cp] UNIQUE NONCLUSTERED 
(
	[ConferenceID] ASC,
	[ParticipantID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
ALTER TABLE [dbo].[utConferenceParticipant]  WITH CHECK ADD  CONSTRAINT [FK_utConference_utConferenceParticipant_U] FOREIGN KEY([ConferenceID])
REFERENCES [dbo].[utConference] ([ID])
GO
ALTER TABLE [dbo].[utConferenceParticipant] CHECK CONSTRAINT [FK_utConference_utConferenceParticipant_U]
GO
ALTER TABLE [dbo].[utConferenceParticipant]  WITH CHECK ADD  CONSTRAINT [FK_utParticipant_utConferenceParticipant_U] FOREIGN KEY([ParticipantID])
REFERENCES [dbo].[utParticipant] ([ID])
GO
ALTER TABLE [dbo].[utConferenceParticipant] CHECK CONSTRAINT [FK_utParticipant_utConferenceParticipant_U]
GO
ALTER TABLE [dbo].[utConferenceParticipant]  WITH CHECK ADD  CONSTRAINT [FK_utParticipantType_utConferenceParticipant_U] FOREIGN KEY([ParticipantType])
REFERENCES [dbo].[utParticipantType] ([ID])
GO
ALTER TABLE [dbo].[utConferenceParticipant] CHECK CONSTRAINT [FK_utParticipantType_utConferenceParticipant_U]
GO
