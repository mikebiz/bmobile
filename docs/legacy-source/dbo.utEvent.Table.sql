/****** Object:  Table [dbo].[utEvent]    Script Date: 08/27/2008 14:35:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[utEvent](
	[ID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[Name] [nvarchar](50) NOT NULL DEFAULT ('Name'),
	[Received] [datetime] NOT NULL DEFAULT (getdate()),
	[Sent] [datetime] NOT NULL DEFAULT (getdate()),
	[CorrelationID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[EventType] [int] NOT NULL DEFAULT ((0)),
	[ConferenceParticipantID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[SentCounter] [int] NOT NULL DEFAULT ((0)),
 CONSTRAINT [PK_utEvent] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[utEvent]  WITH CHECK ADD  CONSTRAINT [FK_utConferenceParticipant_utEvent_U] FOREIGN KEY([ConferenceParticipantID])
REFERENCES [dbo].[utConferenceParticipant] ([ID])
GO
ALTER TABLE [dbo].[utEvent] CHECK CONSTRAINT [FK_utConferenceParticipant_utEvent_U]
GO
ALTER TABLE [dbo].[utEvent]  WITH CHECK ADD  CONSTRAINT [FK_utEventType_utEvent_U] FOREIGN KEY([EventType])
REFERENCES [dbo].[utEventType] ([ID])
GO
ALTER TABLE [dbo].[utEvent] CHECK CONSTRAINT [FK_utEventType_utEvent_U]
GO
