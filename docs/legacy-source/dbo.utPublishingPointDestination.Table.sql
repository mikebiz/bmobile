/****** Object:  Table [dbo].[utPublishingPointDestination]    Script Date: 08/27/2008 14:35:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[utPublishingPointDestination](
	[ID] [uniqueidentifier] NOT NULL CONSTRAINT [DF__utPublishing__ID__55B597A7]  DEFAULT (newid()),
	[ConferenceParticipantID] [uniqueidentifier] NOT NULL CONSTRAINT [DF__utPublish__Confe__56A9BBE0]  DEFAULT (newid()),
	[ipVerLen] [smallint] NOT NULL,
	[ipTos] [smallint] NOT NULL,
	[ipTotalLength] [int] NOT NULL,
	[ipId] [int] NOT NULL,
	[ipOffset] [int] NOT NULL,
	[ipTtl] [smallint] NOT NULL,
	[ipProtocol] [smallint] NOT NULL,
	[srcIP] [bigint] NOT NULL CONSTRAINT [DF_utPublishingPointDestination_srcIP1]  DEFAULT ((0)),
	[destIP] [bigint] NOT NULL CONSTRAINT [DF_utPublishingPointDestination_destIP1]  DEFAULT ((0)),
	[srcPort] [int] NOT NULL CONSTRAINT [DF__utPublish__srcPo__5986288B]  DEFAULT ((0)),
	[destPort] [int] NOT NULL CONSTRAINT [DF__utPublish__destP__5A7A4CC4]  DEFAULT ((0)),
	[udpLength] [int] NOT NULL,
	[isActive] [bit] NULL CONSTRAINT [DF_utPublishingPointDestination_isActive]  DEFAULT ((1)),
	[isAvailable] [bit] NULL CONSTRAINT [DF_utPublishingPointDestination_isStreamable]  DEFAULT ((1)),
	[lastUpdated] [datetime] NULL CONSTRAINT [DF_utPublishingPointDestination_lastUpdated]  DEFAULT (getdate()),
 CONSTRAINT [PK_utPublishingPointDestination] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[utPublishingPointDestination]  WITH CHECK ADD  CONSTRAINT [FK_utConferenceParticipant_utPublishingPointDestination_U] FOREIGN KEY([ConferenceParticipantID])
REFERENCES [dbo].[utConferenceParticipant] ([ID])
GO
ALTER TABLE [dbo].[utPublishingPointDestination] CHECK CONSTRAINT [FK_utConferenceParticipant_utPublishingPointDestination_U]
GO
